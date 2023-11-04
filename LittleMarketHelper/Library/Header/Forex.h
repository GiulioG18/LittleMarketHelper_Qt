
// ========================================================================
//		Container for Forex operations
// ========================================================================

#pragma once

#include "Price.h"


namespace lmh {

	// Forward declarations
	class ExchangeRate;

	// The behaviour of this class is undefined if the exchange rate repository has not been initialized successfully
	class Forex
	{
	public:

		static Price convert(const Price& price, Currency targetCurrency);
		static bool availableCurrency(Currency xxx);
		static bool baseCurrency(Currency xxx);
		static std::string denomination(Currency xxx, Currency yyy);
		static std::string denomination(const ExchangeRate& rate);

	private:

		static ExchangeRate crossCurrencyRate(Currency xxx, Currency yyy);
	};

}