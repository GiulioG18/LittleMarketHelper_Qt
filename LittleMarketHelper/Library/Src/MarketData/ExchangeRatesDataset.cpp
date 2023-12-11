
#include "MarketData\ExchangeRatesDataset.h"
#include "ExchangeRate.h"
#include "Forex.h"
#include "Config.h"
#include "Http/Api.h"


namespace lmh {

	ExchangeRatesDataset::ExchangeRatesDataset(Currency baseCurrency)
		: baseCurrency_(baseCurrency)
	{
		// Initialize base currency and register it as available
		availableCurrencies_.insert(baseCurrency_);
		exchangeRates_.insert({ Forex::denomination(baseCurrency_, baseCurrency_), ExchangeRate(baseCurrency_, baseCurrency_, 1.0) });
	}

	void ExchangeRatesDataset::populateWithLatestQuotes()
	{
		// Fetch latest quotes for various rates
		std::set<ExchangeRate> exchangeRates = http::Api::fetchLatestRatesFor(baseCurrency_);

		// Register them
		for (auto& rate : exchangeRates)
		{
			// Conversion between non-base currencies are triangulated using the
			// base one, therefore only base currency rates are stored
			if (rate.xxx() == baseCurrency_)
			{
				// Skip non-positive rate
				if (rate.value() > 0.0)
				{
					// Insert rate into maps
					bool inserted = exchangeRates_.insert({ Forex::denomination(rate.xxx(), rate.yyy()), rate }).second;

					// Mark currency as available
					if (inserted)
						availableCurrencies_.insert(rate.yyy());
				}
			}
		}

		ENSURE(availableCurrencies_.find(baseCurrency_) != availableCurrencies_.end(), "base currency not registered as available");
	}

}