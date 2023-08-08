
#include <algorithm>

#include "Calibrator.h"


namespace lmh {

	Calibrator::Calibrator(const Portfolio& portfolio)
		: 
		output_(std::nullopt),
		trades_(portfolio.included()),
		balance_(portfolio.balance())
	{
		REQUIRE(trades_, "invalid trades");
		REQUIRE(balance_, "invalid balance");

		registerWith(portfolio.balance());
	}

	bool Calibrator::runOptimization(Input input)
	{
		// Clear old output
		output_ = std::nullopt;
		if (!validateInput(input))
			return false;

		bool successful = true;



		return successful;
	}

	const Calibrator::Output& Calibrator::output() const
	{
		return output_;
	}

	void Calibrator::update()
	{
		output_ = std::nullopt;
	}

	bool Calibrator::validateInput(Input& input)
	{
		if (input.size() != trades_->trades().size())
			return false;

		float weightSum = 0.0f;
		for (const auto& trade : trades_->trades())
		{
			// Check if input products match portfolio trades
			std::string name = trade.first->name();
			auto it = input.find(name);
			if (it == input.end())
				return false;

			// Extract price
			it->second.price_ = trade.first->price();
			REQUIRE(!Null<float>::check(it->second.price_), "null price");

			// Validate target weights
			const float& weight = it->second.targetWeight_;
			if (weight > 1 || weight < 0)
				return false;
			weightSum += weight;
		}
		// Check if sum of target weights is 1
		if (weightSum - 1.0f > FLT_EPSILON)
			return false;

		return true;
	}

}