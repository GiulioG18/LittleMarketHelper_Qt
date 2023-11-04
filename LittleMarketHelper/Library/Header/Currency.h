// ========================================================================
//		Currencies
// ========================================================================

#pragma once

#include <string>
#include <map>
#include <optional>

#include "Patterns/Observable.h"


namespace lmh {

	enum class Currency
	{
		EUR,
		USD,
		GBP,
		JPY,
		CHF,
		AUD,
		CAD,
		CNY,
		HKD,
		SEK,
		BZR
	};

	namespace ccy {

		// Utilities

		// Get string from currency
		std::string	ctos(Currency currency);
		// Get currency from string (returns std::nullopt if there is no matching currency)
		std::optional<Currency> stoc(const std::string& currencyStr);
		// Returns a map of all currencies
		const std::map<Currency, std::string>& map();
	}

}
