
#include "ExchangeRate.h"


namespace lmh {

#define RATES_STORED_SIZE 20

	// ExchangeRate

	ExchangeRate::ExchangeRate(Currency::Type xxx, Currency::Type yyy)
		:
		xxx_(xxx),
		yyy_(yyy),
		rate_(nullptr)
	{
		REQUIRE(xxx_ != yyy_, "same currency provided for exchange rate ccys");
	}


	// ExchangeRateRepository

	ExchangeRateRepository::ExchangeRateRepository()
		: rates_(std::make_unique<FifoCache<std::string, ExchangeRate>>(RATES_STORED_SIZE))
	{
		// TODO: fetch most common rates
		// TODO: set available ccys
		// TODO: register exchange rates
	}

	Price ExchangeRateRepository::convertPrice(const Price& price, Currency::Type ccy)
	{
		double out = 0.0;
		const Currency::Type& xxx = price.ccy();
		const Currency::Type& yyy = ccy;

		if (xxx == yyy)
			out = price.value();
		else
			out = price.value() / getConversionRate(xxx, yyy);


		ENSURE(out > 0, "invalid translated value");
		return { xxx, out };
	}

	std::string ExchangeRateRepository::denomination(Currency::Type xxx, Currency::Type yyy) const
	{
		return Currency::toString(xxx) + Currency::toString(yyy);
	}

	bool ExchangeRateRepository::isAvailable(Currency::Type xxx) const
	{
		return availableCcys_.contains(xxx);
	}

	double ExchangeRateRepository::getConversionRate(Currency::Type xxx, Currency::Type yyy) const
	{
		REQUIRE(isAvailable(xxx), "currency not available");
		REQUIRE(isAvailable(yyy), "currency not available");

		auto exchangeRate = rates_->get(denomination(xxx, yyy));
		ASSERT(exchangeRate, "exchange rate has not been registered");
		ASSERT(exchangeRate->rate(), "rate has not been set");

		// TODO: figure out how to handle triangulation
		return *exchangeRate->rate();
	}

}