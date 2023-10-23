
#include <algorithm>

#include "Utils/Assertions.h"
#include "CalibrationResult.h"


namespace lmh {

	CalibrationResult::CalibrationResult(Currency::Type ccy)
		: 
		ccy_(ccy),
		investment_(0.0),
		openPosition_(0.0),
		cash_(0.0),
		initialized_(false)
	{
	}

	bool CalibrationResult::initialize(const WeightsMap& wm, double investment, const std::set<std::shared_ptr<WSecurity>, WSecurity::Comp>& securities)
	{
		if (initialized_)
			return true;

		data_.reserve(wm.size());
		for (const auto& isinWeight : wm)
		{
			const std::string& isin = isinWeight.first;
			const double& weight = isinWeight.second;

			// Extract price
			const auto& it = securities.find(isin);
			ASSERT(it != securities.end(), "security-input mismatch");			
			double price = it->get()->quote().price().value();
			ASSERT(price > 0.0, "invalid price");

			// Create datum from weight map entry
			data_.emplace_back(
				isin, price, weight,
				0, 0.0, 0
			);
		}

		// Initialize investment
		investment_ = investment;

		initialized_ = true;
		return initialized_;
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
		cash_ = openPosition_ = 0.0;
	}

	// TODO: review this function. it seems that if no optimization is run
	// then the Data validation is still successful.. is this behaviour wanted?
	bool CalibrationResult::validate() const
	{
		ASSERT(initialized_, "un-initialized results");

		// Data validation
		bool validated = true;
		double realSum = 0.0;
		double idealSum = 0.0;
		for (const auto& datum : data_)
		{
			if (!datum.validate())
			{
				validated = false;
				break;
			}

			idealSum += datum.idealWeight();
			realSum += datum.realWeight();
		}
		if (!validated || idealSum - 1.0 > DBL_EPSILON || realSum - 1.0 > DBL_EPSILON)
			return false;

		// Investment, cash, open position validation
		if (openPosition_ != investment_ - cash_) // TODO: floating point comparison should use closeEnough
			return false;
		if (investment_ < 0 ||
			cash_ < 0 || 
			openPosition_ < 0)
			return false;

		return true;
	}



	CalibrationResult::Datum::Datum(
		std::string name,
		double price,
		double idealWeight,
		int idealQuantity,
		double realWeight,
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
			(realWeight_ >= 0.0 || realWeight_ <= 1.0) &&
			realQuantity_ >= 0;
	}

	void CalibrationResult::Datum::partialReset()
	{
		idealQuantity_ = 0;
		realWeight_ = 0.0;
		realQuantity_ = 0;
	}

}