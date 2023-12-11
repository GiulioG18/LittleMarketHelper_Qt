
#include "Utils/Assertions.h"
#include "ExchangeRate.h"
#include "Forex.h"


namespace lmh {

	ExchangeRate::ExchangeRate(std::string_view denomination, double value)
		:
		xxx_(Currency::EUR),
		yyy_(Currency::EUR),
		denomination_(denomination),
		value_(value)
	{
		REQUIRE(denomination.length() == 6, "invalid denomination");

		// Extract xxx
		auto xxx = ccy::stoc(denomination.substr(0, 3));
		ASSERT(xxx.has_value(), "invalid xxx currency in denomination");
		xxx_ = xxx.value();

		// Extract yyy
		auto yyy = ccy::stoc(denomination.substr(3, 3));
		ASSERT(yyy.has_value(), "invalid yyy currency in denomination");
		yyy_ = yyy.value();

		// Validate 
		ENSURE(value_ > 0, "exchange rate provided is not positive");
		ENSURE(denomination_.length() == 6, "invalid denomination");
		if (xxx_ == yyy_)
			ENSURE(value_ == 1.0, "same currency exchange rate is not 1");
	}

	ExchangeRate::ExchangeRate(Currency xxx, Currency yyy, double value)
		:
		xxx_(xxx),
		yyy_(yyy),
		value_(value),
		denomination_(Forex::denomination(xxx, yyy))
	{
		// Validate 
		ENSURE(value_ > 0, "exchange rate provided is not positive");
		ENSURE(denomination_.length() == 6, "invalid denomination");
		if (xxx_ == yyy_)
			ENSURE(value_ == 1.0, "same currency exchange rate is not 1");
	}

	bool ExchangeRate::operator<(const ExchangeRate& other) const
	{
		return this->denomination_ < other.denomination_;
	}

}