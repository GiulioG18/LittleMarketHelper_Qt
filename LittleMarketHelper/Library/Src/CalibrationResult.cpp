
#include <algorithm>

#include "CalibrationResult.h"
#include "Security.h"
#include "Utils/Assertions.h"


namespace lmh {

	bool CalibrationResult::initialize(const WeightsMap& wm, float investment, Tradeset* tradeset)
	{
		if (initialized_)
			return true;

		// Initialize data
		if (wm.empty())
			return false;
		data_.reserve(wm.size());
		for (const auto& isinWeight : wm)
		{
			const std::string& isin = isinWeight.first;
			const float& weight = isinWeight.second;

			// Extract price
			const auto& it = tradeset->find(isin);
			ASSERT(it != tradeset->get().end(), "trades-inputs mismatch");
			float price = it->first->price();
			ASSERT(Security::validatePrice(price), "not valid price");

			// Create datum from weight map entry
			data_.emplace_back(
				isin, price, weight,
				0, 0.0f, 0
			);
		}

		// Initialize investment
		investment_ = investment;

		initialized_ = true;
		return true;
	}

	void CalibrationResult::partialReset()
	{
		ASSERT(initialized_, "un-initialized results");

		// Reset non-const data members
		std::for_each(std::begin(data_), std::end(data_), 
			[](CalibrationResult::Datum& d)
			{
				d.partialReset();
			}
		);

		// Reset cash, open position
		cash_ = openPosition_ = 0.0f;
	}

	bool CalibrationResult::validate() const
	{
		ASSERT(initialized_, "un-initialized results");

		// Data
		bool validated = true;
		std::for_each(std::begin(data_), std::end(data_), 
			[&validated](const CalibrationResult::Datum& d)
			{
				if (!d.validate())
					validated = false;
			}
		);
		if (!validated)
			return false;

		// Investment, cash, open position
		ENSURE(openPosition_ == investment_ - cash_,
			"open position is incorrect");
		if (investment_ < 0 ||
			cash_ < 0 || 
			openPosition_ < 0)
			return false;

		return true;
	}



	CalibrationResult::Datum::Datum(
		std::string name,
		float price,
		float idealWeight,
		int idealQuantity,
		float realWeight,
		int realQuantity)
		:
		name_(name), price_(price),
		idealWeight_(idealWeight), idealQuantity_(idealQuantity),
		realWeight_(realWeight), realQuantity_(realQuantity)
	{
	};

	bool CalibrationResult::Datum::validate() const
	{
		return
			idealQuantity_ >= 0 &&
			(realWeight_ >= 0 || realWeight_ <= 1) &&
			realQuantity_ >= 0;
	}

	void CalibrationResult::Datum::partialReset()
	{
		idealQuantity_ = 0;
		realWeight_ = 0.0f;
		realQuantity_ = 0;
	}

}