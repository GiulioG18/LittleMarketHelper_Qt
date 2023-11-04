
#include "Forex.h"
#include "ExchangeRate.h"


namespace lmh {

	Price Forex::convert(const Price& price, Currency targetCurrency)
	{
		// Example: Converting 100USD into EUR returns: 100 / EURUSD
		double rate = crossCurrencyRate(targetCurrency, price.currency()).quote().price().value();
		double value = price.value() / rate;
		ENSURE(value > 0.0, "invalid translated value"); // TODO: ensure, since the value is always positive, and exchange rate is also checked at construction

		return { targetCurrency, value };
	}

	bool Forex::availableCurrency(Currency Currency)
	{
		return ExchangeRateRepository::get().availableCurrencies_.contains(Currency);
	}

	bool Forex::baseCurrency(Currency xxx)
	{
		return xxx == ExchangeRateRepository::get().baseCurrency_;
	}

	std::string Forex::denomination(Currency xxx, Currency yyy)
	{
		return ccy::ctos(xxx) + ccy::ctos(yyy);
	}

	std::string Forex::denomination(const ExchangeRate& rate)
	{
		return denomination(rate.xxx(), rate.yyy());
	}

	ExchangeRate Forex::crossCurrencyRate(Currency xxx, Currency yyy)
	{
		REQUIRE(availableCurrency(xxx) && availableCurrency(yyy), "conversion for non-available currencies"); // TODO: require...
		const ExchangeRateRepository& repo = ExchangeRateRepository::get();
		double rate = -1.0;

		// 3 cases:
		// - XXX is base currency
		// - YYY is base currency
		// - triangulate using base currency

		if (baseCurrency(xxx))
		{
			// Simply get the rate from the map			
			rate = repo.rates_.at(denomination(xxx, yyy)).quote().price().value();			
		}
		else if (baseCurrency(yyy))
		{
			// Get the reciprocal of XXXYYY (since it is garanteed to exist in the map)
			rate = 1 / repo.rates_.at(denomination(yyy, xxx)).quote().price().value();
		}
		else
		{
			// XXXYYY = (BBBYYY / BBBXXX)
			double bbbyyy = repo.rates_.at(denomination(repo.baseCurrency_, yyy)).quote().price().value();
			double bbbxxx = repo.rates_.at(denomination(repo.baseCurrency_, xxx)).quote().price().value();
			rate = bbbyyy / bbbxxx;
		}

		return ExchangeRate(xxx, yyy, Quote({ yyy, rate }, 0.0)); // TODO: set timestamp equal to oldest timestamp between the rates used
	}

}