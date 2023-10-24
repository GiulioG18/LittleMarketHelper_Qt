// ========================================================================
//		Exchange Rate class
// ========================================================================

#pragma once

#include <memory>

#include "Patterns/Singleton.h"
#include "Utils/Cache.h"
#include "Currency.h"
#include "Price.h"


namespace lmh {

	class ExchangeRate
	{
	public:

		ExchangeRate(Currency::Type xxx, Currency::Type yyy);

		// Getters
		inline Currency::Type xxx() const;
		inline Currency::Type yyy() const;
		inline const std::unique_ptr<double>& rate() const;

	private:

		// Setters // TODO: remove
		inline void setRate(double r);

		Currency::Type xxx_;
		Currency::Type yyy_;
		std::unique_ptr<double> rate_;
	};


	class ExchangeRateRepository : public Singleton<ExchangeRateRepository>
	{
	public:

		using CachedRates = std::unique_ptr<Cache<std::string, ExchangeRate>>;

	public:

		// TODO: create an interface for the user (even just a utility class with static functions, currency class would work with a diff name that i always wanted...) 
		//		 and keep the repo hidden to the user
		// TODO: should have a function that put()s in the cache the most common ER in a single request

		ExchangeRateRepository();

		Price convertPrice(const Price& price, Currency::Type ccy);
		std::string denomination(Currency::Type xxx, Currency::Type yyy) const;
		bool isAvailable(Currency::Type xxx) const;

	private:

		double getConversionRate(Currency::Type xxx, Currency::Type yyy) const;

	private:

		std::set<Currency::Type> availableCcys_;
		CachedRates rates_;
	};


	// Inline definitions
	inline Currency::Type ExchangeRate::xxx() const { return xxx_; }
	inline Currency::Type ExchangeRate::yyy() const { return yyy_; }
	inline const std::unique_ptr<double>& ExchangeRate::rate() const { return rate_; }
	inline void ExchangeRate::setRate(double r) { REQUIRE(r > 0, "exchange rate provided is not positive");  *rate_ = r; }

}
