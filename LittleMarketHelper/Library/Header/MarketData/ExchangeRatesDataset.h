// ========================================================================
//		Exchange rates dataset
// ========================================================================

#pragma once

#include <unordered_map>
#include <set>
#include <string_view>

#include "Utils/StatusCode.h"
#include "Currency.h"
#include "Quote.h"


namespace lmh {
	
	// Forward declarations
	class ExchangeRate;


	class ExchangeRatesDataset
	{
	public:

		ExchangeRatesDataset(Currency baseCurrency);

		void populateWithLatestQuotes();

		// Getters
		const std::set<Currency>& availableCurrencies() const;

	private:

		std::unordered_map<std::string, const ExchangeRate> exchangeRates_;
		std::set<Currency> availableCurrencies_;
		Currency baseCurrency_;
	};

}
