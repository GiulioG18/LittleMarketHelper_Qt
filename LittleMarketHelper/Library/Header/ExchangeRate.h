// ========================================================================
//		Exchange Rate class
// ========================================================================

#pragma once

#include <unordered_map>
#include <memory>

#include "Patterns/Singleton.h"
#include "Utils/StatusCode.h"
#include "Currency.h"
#include "Quote.h"
#include "Forex.h"


namespace lmh {

	class ExchangeRate
	{
	public:
		
		ExchangeRate(std::string denomination, const Quote& rate); // [ MAY THROW ]
		ExchangeRate(Currency xxx, Currency yyy, const Quote& rate); // [ MAY THROW ]

		// Getters
		inline Currency xxx() const;
		inline Currency yyy() const;
		inline const Quote& quote() const;

		bool operator<(const ExchangeRate& other) const;

	private:

		std::string denomination_;
		Currency xxx_;
		Currency yyy_;
		Quote quote_; // Quote::price_ refers to the non-base currency
	};


	class ExchangeRateRepository : public Singleton<ExchangeRateRepository>
	{
	public:
		
		friend class Forex;

		using RatesMap = std::unordered_map<std::string, const ExchangeRate>;

	public:

		ExchangeRateRepository() = default;
		// Initializes rate map and available currencies
		// NB: the base currency selected here should match the one in the config file
		static Status initialize(Currency baseCurrency = Currency::EUR);

	private:

		std::set<Currency> availableCurrencies_; // TODO: ensure that this is actually constant throughout the whole application, otherwise there could be unavailable rates for available Currencys
		RatesMap rates_;
		Currency baseCurrency_ = Currency::EUR;
		bool initialized_ = false;
	};


	// Inline definitions
	inline Currency ExchangeRate::xxx() const { return xxx_; }
	inline Currency ExchangeRate::yyy() const { return yyy_; }
	inline const Quote& ExchangeRate::quote() const { return quote_; }

}
