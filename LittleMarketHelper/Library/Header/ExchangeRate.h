// ========================================================================
//		Exchange Rate class
// ========================================================================

#pragma once

#include <memory>

#include "Patterns/Singleton.h"
#include "Utils/Cache.h"
#include "Currency.h"
#include "Quote.h"


namespace lmh {

	class ExchangeRate
	{
	public:

		// [ MAY THROW ]
		ExchangeRate(std::string denomination, const Quote& rate);
		// [ MAY THROW ]
		ExchangeRate(Currency::Type xxx, Currency::Type yyy, const Quote& rate);

		// Getters
		inline Currency::Type xxx() const;
		inline Currency::Type yyy() const;
		inline const Quote& rate() const;

		bool operator<(const ExchangeRate& other) const;

	private:

		std::string denomination_;
		Currency::Type xxx_;
		Currency::Type yyy_;
		Quote rate_; // Quote::price_ refers to the non-base ccy
	};


	class ExchangeRateRepository : public Singleton<ExchangeRateRepository>
	{
	public:

		using CachedRates = std::unique_ptr<Cache<std::string, ExchangeRate>>;

	public:

		// TODO: create an interface for the user (even just a utility class with static functions, currency class would work with a diff name that i always wanted...) 
		//		 and keep the repo hidden to the user (Forex ???)

		ExchangeRateRepository();

		Price convert(const Price& price, Currency::Type ccy);
		bool isAvailable(Currency::Type xxx) const;

	private:

		// TODO: right now it handles only cases where xxx is EUR
		// [ MAY THROW ]
		double getConversionRate(Currency::Type xxx, Currency::Type yyy) const;

	private:

		std::set<Currency::Type> availableCcys_;
		CachedRates rates_;
		CachedRates invertedRates_; // Value points to the original rate
	};


	// Inline definitions
	inline Currency::Type ExchangeRate::xxx() const { return xxx_; }
	inline Currency::Type ExchangeRate::yyy() const { return yyy_; }
	inline const Quote& ExchangeRate::rate() const { return rate_; }

}
