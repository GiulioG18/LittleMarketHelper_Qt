
#include <algorithm>

#include "CalibrationResult.h"


namespace lmh {

	bool CalibrationResult::initialize(const WeightsMap& wm, float investment, Tradeset* tradeset)
	{
		// Initialize data
		if (wm.empty())
			return false;
		data_.reserve(wm.size());
		for (const auto& NameWeight : wm)
		{
			// Extract price
			const auto& it = tradeset->find(NameWeight.first);
			ASSERT(it != tradeset->get().end(), "trades-inputs mismatch");
			float price = it->first->price();
			ASSERT(!Null<float>::check(price), "null price");

			// Create datum from weight map entry
			data_.emplace_back(
				NameWeight.first, price, NameWeight.second,
				Null<int>(), Null<float>(), Null<int>()
			);
		}

		// Initialize investment
		investment_ = investment;

		return true;
	}

	void CalibrationResult::partialReset()
	{
		// Reset non-const data members
		std::for_each(std::begin(data_), std::end(data_), 
			[](CalibrationResult::Datum& d)
			{
				d.partialReset();
			}
		);

		// Reset cash, open position
		cash_ = openPosition_ = Null<float>();
	}

	bool CalibrationResult::validate() const
	{
		// Data
		bool validated = true;
		std::for_each(std::begin(data_), std::end(data_), 
			[&validated](const CalibrationResult::Datum& d)
			{
				if (!d.checkNull())
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
		if (Null<float>::check(cash_) ||
			Null<float>::check(openPosition_))
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

	bool CalibrationResult::Datum::checkNull() const
	{
		return
			!Null<int>::check(idealQuantity_) &&
			!Null<float>::check(realWeight_) &&
			!Null<int>::check(realQuantity_);
	}

	void CalibrationResult::Datum::partialReset()
	{
		idealQuantity_ = Null<int>();
		realWeight_ = Null<float>();
		realQuantity_ = Null<int>();
	}

}