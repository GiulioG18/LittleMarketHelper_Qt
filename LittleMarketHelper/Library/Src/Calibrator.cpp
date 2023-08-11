
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

	bool Calibrator::runOptimization(Input input, float amountToInvest)
	{
		// Clear old output
		output_ = std::nullopt;

		if (!validateInput(input))
			return false;


		float total = Null<float>::check(amountToInvest) ?
			balance_->value() :	// Use the balance as total
			amountToInvest;		// Use the customized amount as total
		if (total <= 0.0f)
			return false;

		bool successful = true;

		// Start optimization
		
		// Empty because of Arvind...
		//...
		//...

		if (true/*!successful*/)
			return NaiveFallback(input, total);
	}

	const Calibrator::Output& Calibrator::output() const
	{
		return output_;
	}

	void Calibrator::update()
	{
		output_ = std::nullopt;
	}

	bool Calibrator::NaiveFallback(const Input& input, float amountToInvest)
	{
		// Fill data from input
		std::vector<ODatum> optData;
		for (const auto& i : input)
		{
			static int counter = 0;

			int index = counter++;
			std::string name = i.first;
			float price = i.second.price_;
			float idealWeight = i.second.targetWeight_;
			float realWeight = Null<float>();		// To be computed
			int idealQuantity = Null<int>();		// To be computed
			int realQuantity = Null<int>();			// To be computed

			optData.push_back(ODatum(index, name, price, idealWeight, realWeight, idealQuantity, realQuantity));
		}
		// Sort based on price.
		// This actually should not have much of an impact, since
		// we are flooring the ideal quantity (flooring will cause to 
		// under-invest the available amount, but allows enough cash for all products
		std::sort(std::begin(optData), std::end(optData), [](const ODatum& i, const ODatum& j)
			{
				return i.price_ > j.price_;
			}
		);

		// Fill ideal quantities
		std::for_each(std::begin(optData), std::end(optData), [amountToInvest](ODatum& o)
			{
				o.idealQuantity_ = static_cast<int>(std::floorf(o.idealWeight_ * amountToInvest / o.price_));
			}
		);

		// Fill real quantities
		float total = 0.0f;
		std::for_each(std::begin(optData), std::end(optData), [&total, amountToInvest](ODatum& o)
			{
				float newTotal = total + o.idealQuantity * o.price;

				if (newTotal > amountToInvest)
				{
					o.realQuantity = static_cast<int>(std::floorf((amountToInvest - total) / o.price));
				}
				else
				{
					o.realQuantity = o.idealQuantity;
				}

				total = newTotal;
			}
		);

		// Fill real weights
		std::for_each(std::begin(optData), std::end(optData), [total](OptDatum_& o)
			{
				o.realWeight = o.realQuantity * o.price / total;
			}
		);

		// Validate results
		bool successful = true;
		std::for_each(std::begin(optData), std::end(optData), [&successful](OptDatum_& o)
			{
				successful =
						(!Null<float>::check(o.realWeight) &&
						!Null<int>::check(o.idealQuantity) &&
						!Null<int>::check(o.realQuantity));
			}
		);

		return successful;
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

		// Check balance
		if (balance_->value() <= 0)
			return false;

		return true;
	}

}