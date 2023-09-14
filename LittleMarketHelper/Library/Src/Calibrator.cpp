
#include <memory>
#include <fstream>

#include "Calibrator.h"
#include "Optimizers.h"


namespace lmh {

	Calibrator::Calibrator(const Portfolio& portfolio)
		: 
		result_(std::nullopt),
		trades_(portfolio.trades()),
		balance_(portfolio.balance())
	{
		REQUIRE(trades_, "invalid trades");
		REQUIRE(balance_, "invalid balance");

		registerWith(portfolio.balance());
	}

	void Calibrator::runOptimization(WeightsMap wm, float investment)
	{
		// Clear old output
		clearResult();

		if (!validateWeightsMap(wm))
			return;

		// If investment amount is not provided, use the linked portfolio 
		// balance as cash constraint for the optimization
		float inv = investment == 0.0f ?
			balance_->value() :
			investment;		
		if (inv <= 0.0f)
			return;

		CalibrationResult result;
		bool successful= result.initialize(
			wm, 
			inv,
			trades_.get());

		if (!successful)
			return;

		// Run SCIP optimization
		successful = SCIP(result).run();
		// If fails, try the naive optimization
		if (!successful)
			successful = Floored(result).run();;

		if (successful)
			setResult(std::move(result));
	}

	void Calibrator::update()
	{
		clearResult();
	}

	void Calibrator::clearResult()
	{
		result_ = std::nullopt;
	}

	void Calibrator::setResult(CalibrationResult&& result)
	{
		result_ = std::make_optional(std::move(result));
	}

	bool Calibrator::validateWeightsMap(const WeightsMap& wm) const
	{
		// Check if input weights match portfolio trades 
		if (wm.size() != trades_->get().size())
			return false;

		float weightSum = 0.0f;
		for (const auto& trade : trades_->get())
		{
			// Check that it is provided 1 weight per security
			std::string name = trade.first->name();
			auto it = wm.find(name);
			if (it == wm.end())
				return false;

			// Check ideal weights
			const float& weight = it->second;
			if (weight > 1 || weight < 0)
				return false;
			weightSum += weight;
		}

		// Check if sum of ideal weights is 1.0
		if (weightSum - 1.0f > FLT_EPSILON)
			return false;

		return true;
	}
}