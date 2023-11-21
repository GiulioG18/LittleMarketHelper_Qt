
// ========================================================================
//		Container for Forex operations
// ========================================================================

#pragma once

#include <string>


namespace lmh {

	// Forward declarations
	class ExchangeRate;
	class Price;
	enum class Currency;

	// The behaviour of this class is undefined if the exchange rate repository has not been initialized successfully
	class Forex
	{
	public:

		static Price convert(const Price& price, Currency targetCurrency);
		static bool availableCurrency(Currency currency);
		static bool baseCurrency(Currency currency);
		static std::string denomination(Currency xxx, Currency yyy);
		static std::string denomination(const ExchangeRate& rate);

	private:

		static ExchangeRate crossCurrencyRate(Currency xxx, Currency yyy);
	};

}