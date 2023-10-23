
#include <memory>
#include <fstream>

#include "Calibrator.h"
#include "Optimizers.h"


namespace lmh {

	Calibrator::Calibrator(const Portfolio* const portfolio)
		: 
		result_(std::nullopt),
		portfolio_(portfolio)
	{
		REQUIRE(portfolio->openPosition(), "invalid open position");

		registerWith(portfolio->openPosition());
	}

	void Calibrator::runOptimization(WeightsMap wm, Currency::Type ccy, double investment)
	{
		// Clear old output
		clearResult();

		// Validate inputs
		if (!validateWeightsMap(wm) || investment <= 0.0)
			return;

		CalibrationResult result(ccy);
		bool successful = result.initialize(
			wm, 
			investment,
			portfolio_->securities());

		if (!successful)
			return;

		// Run SCIP optimization
		successful = SCIP(result).run();
		// If fails, try the naive optimization
		if (!successful)
			successful = Floored(result).run();

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
		REQUIRE(portfolio_, "invalid portfolio");

		// Check if input weights match portfolio securities and there is at least 1 security
		if (wm.size() != portfolio_->size() || wm.size() == 0)
			return false;

		double weightSum = 0.0;
		for (const auto& security : portfolio_->securities())
		{
			// Check that it is provided 1 weight per security
			std::string isin = security->isin();
			auto it = wm.find(isin);
			if (it == wm.end())
				return false;

			// Check ideal weights range
			const double& weight = it->second;
			if (weight > 1.0 || weight < 0.0)
				return false;
			weightSum += weight;
		}

		// Check if sum of ideal weights is 1.0
		if (weightSum - 1.0 > DBL_EPSILON)
			return false;

		return true;
	}
}