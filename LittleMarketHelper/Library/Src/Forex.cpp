
#include <assert.h>

#include "Forex.h"
#include "ExchangeRate.h"
#include "Price.h"


namespace lmh {

	Price Forex::convert(const Price& price, Currency targetCurrency)
	{
		// Example: Converting 100USD into EUR returns: 100 / EURUSD
		double rate = crossCurrencyRate(targetCurrency, price.currency()).value();
		double value = price.amount() / rate;
		assert(value > 0.0);

		return { targetCurrency, value };
	}

	bool Forex::availableCurrency(Currency Currency)
	{
		return ExchangeRateRepository::get().availableCurrencies_.contains(Currency);
	}

	bool Forex::baseCurrency(Currency currency)
	{
		return currency == ExchangeRateRepository::get().baseCurrency_;
	}

	std::string Forex::denomination(Currency xxx, Currency yyy)
	{
		std::string denom = ccy::ctos(xxx) + ccy::ctos(yyy);

		ENSURE(denom.length() == 6, "invalid denomination");
		return denom;
	}

	std::string Forex::denomination(const ExchangeRate& rate)
	{
		return denomination(rate.xxx(), rate.yyy());
	}

	ExchangeRate Forex::crossCurrencyRate(Currency xxx, Currency yyy)
	{
		assert(availableCurrency(xxx) && availableCurrency(yyy));
		const ExchangeRateRepository& repo = ExchangeRateRepository::get();
		double rate = -1.0;

		// 3 cases:
		// - XXX is base currency
		// - YYY is base currency
		// - triangulate using base currency

		if (baseCurrency(xxx))
		{
			// Simply get the rate from the map			
			rate = repo.rates_.at(denomination(xxx, yyy)).value();			
		}
		else if (baseCurrency(yyy))
		{
			// Get the reciprocal of XXXYYY (since it is garanteed to exist in the map)
			rate = 1 / repo.rates_.at(denomination(yyy, xxx)).value();
		}
		else
		{
			// XXXYYY = (BBBYYY / BBBXXX)
			double bbbyyy = repo.rates_.at(denomination(repo.baseCurrency_, yyy)).value();
			double bbbxxx = repo.rates_.at(denomination(repo.baseCurrency_, xxx)).value();
			rate = bbbyyy / bbbxxx;
		}

		return ExchangeRate(xxx, yyy, rate);
	}

}