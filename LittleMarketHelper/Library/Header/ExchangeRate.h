// ========================================================================
//		Exchange Rate class
// ========================================================================

#pragma once

#include <unordered_map>
#include <memory>
#include <string_view>

#include "Patterns/Singleton.h"
#include "Utils/StatusCode.h"
#include "Currency.h"
#include "Quote.h"
#include "Forex.h"


namespace lmh {

	// Exchange Rate

	class ExchangeRate
	{
	public:
		
		ExchangeRate(std::string_view denomination, double value); // [ MAY THROW ]
		ExchangeRate(Currency xxx, Currency yyy, double value); // [ MAY THROW ]

		// Getters
		inline Currency xxx() const;
		inline Currency yyy() const;
		inline double value() const;

		bool operator<(const ExchangeRate& other) const;

	private:

		std::string denomination_;
		Currency xxx_;
		Currency yyy_;
		double value_;
	};




	// Exchange Rate Repository 

	class ExchangeRateRepository : public Singleton<ExchangeRateRepository>
	{
	public:
		
		friend class Forex;

		using RatesMap = std::unordered_map<std::string, const ExchangeRate>;

	public:

		ExchangeRateRepository() = default;
		// Initializes rate map and available currencies
		// NB: the base currency selected here should match the one in the config file
		static Status initialize(Currency baseCurrency);

	private:

		std::set<Currency> availableCurrencies_;
		RatesMap rates_;
		Currency baseCurrency_ = Currency::EUR;
		bool initialized_ = false;
	};


	// Inline definitions
	inline Currency ExchangeRate::xxx() const { return xxx_; }
	inline Currency ExchangeRate::yyy() const { return yyy_; }
	inline double ExchangeRate::value() const { return value_; }

}
