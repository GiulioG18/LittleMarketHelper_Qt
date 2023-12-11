
#include <cassert>

#include "Utils/Assertions.h"
#include "ExchangeRate.h"
#include "Forex.h"
#include "Quote.h"


namespace lmh {

	Quote Forex::convertQuote(const Quote& quote, Currency targetCurrency)
	{
		// Example: Converting 100USD into EUR returns: 100 / EURUSD
		double rate = getExchangeRateFor(targetCurrency, quote.currency()).value();
		double value = quote.price() / rate;
		assert(value > 0.0);

		return { value, targetCurrency };
	}

	bool Forex::availableCurrency(Currency Currency)
	{
		// TODO: impl
		//return ExchangeRateRepository::get().availableCurrencies_.contains(Currency);
		return false;
	}

	bool Forex::baseCurrency(Currency currency)
	{
		// TODO: impl
		//return currency == ExchangeRateRepository::get().baseCurrency_;
		return false;
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

	ExchangeRate const* Forex::getExchangeRateFor(Currency xxx, Currency yyy)
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