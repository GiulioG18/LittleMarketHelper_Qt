
#include "ExchangeRate.h"
#include "Http/Api.h"


namespace lmh {

	// TODO: create some ex rate utils class and move there

	std::string denomination(Currency::Type xxx, Currency::Type yyy)
	{
		return Currency::toString(xxx) + Currency::toString(yyy);
	}

	std::string denomination(const ExchangeRate& rate)
	{
		return Currency::toString(rate.xxx()) + Currency::toString(rate.yyy());
	}

	// ExchangeRate

	ExchangeRate::ExchangeRate(std::string denomination, const Quote& rate)
		: denomination_(denomination), rate_(rate)
	{
		// TODO: impl
		FAIL("not implemented");
		REQUIRE(xxx_ != yyy_, "same currency provided for exchange rate ccys");
	}

	ExchangeRate::ExchangeRate(Currency::Type xxx, Currency::Type yyy, const Quote& rate)
		:
		xxx_(xxx),
		yyy_(yyy),
		rate_(rate),
		denomination_(denomination(xxx, yyy))
	{
		REQUIRE(rate_.price().value() > 0, "exchange rate provided is not positive");
		REQUIRE(rate_.price().ccy() == yyy_, "quote currency does not match YYY");
		if (xxx_ == yyy_) 
			REQUIRE(rate_.price().value() == 1.0, "same currency exchange rate is not 1");
	}

	bool ExchangeRate::operator<(const ExchangeRate& other) const
	{
		return this->denomination_ < other.denomination_;
	}


	// ExchangeRateRepository

	ExchangeRateRepository::ExchangeRateRepository()
		: rates_(std::make_unique<FifoCache<std::string, ExchangeRate>>())
	{
		// TODO: move into static initialize function to follow the rest of code pattern, this grants more control on when this is called
		//		 by avoid calling the constructor accidentally before Http/Logger stuff is initialized

		// Request EUR rates
		std::set<ExchangeRate> rates = http::ExchangeRate().run();

		for (const auto& rate : rates)
		{
			// Conversion between non-base currencies are triangulated using the base one, 
			// therefore only base currency rates are cached
			if (rate.xxx() != Currency::Type::EUR)	// TODO: should we not hard code base ccy?
				continue;

			// Cache rate
			rates_->			put(denomination(rate.xxx(), rate.yyy()), rate);
			//invertedRates_->	put(denomination(rate.yyy(), rate.xxx()), rates_.get());

			// Mark currency as available
			availableCcys_.insert(rate.yyy());
		}

		ENSURE(false, ""); // TODO: make sure that the invertedRates_ values are the same..
		ENSURE(availableCcys_.find(Currency::Type::EUR) != availableCcys_.end(), "base currency not registered as available"); // TODO: should we not hard code base ccy?
	}


	Price ExchangeRateRepository::convert(const Price& price, Currency::Type ccy)
	{
		double out = 0.0;
		const Currency::Type& xxx = ccy; 
		const Currency::Type& yyy = price.ccy();

		if (xxx == yyy)
			out = price.value();
		else
			out = price.value() / getConversionRate(xxx, yyy);

		ENSURE(out > 0.0, "invalid translated value");
		return { xxx, out };
	}

	bool ExchangeRateRepository::isAvailable(Currency::Type xxx) const
	{
		return availableCcys_.contains(xxx);
	}

	double ExchangeRateRepository::getConversionRate(Currency::Type xxx, Currency::Type yyy) const
	{
		// TODO: handle triangulation
		// TODO: handle case when we have the reverse rate only, cause right now it is failing

		REQUIRE(isAvailable(xxx), "currency not available");
		REQUIRE(isAvailable(yyy), "currency not available");
		// TODO: should we not hard code base ccy? also once the above todo is done, this condition has to be removed
		REQUIRE(xxx == Currency::Type::EUR, "currency conversion not supported, xxx must be EUR"); 

		auto exchangeRate = rates_->get(denomination(xxx, yyy));
		ASSERT(exchangeRate, "exchange rate has not been registered");
		ASSERT(exchangeRate->rate().price().value() > 0.0, "rate has not been set");
		
		return exchangeRate->rate().price().value();
	}

}