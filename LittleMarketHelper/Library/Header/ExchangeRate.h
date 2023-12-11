
// ========================================================================
//		Forex exchange rate
// ========================================================================

#pragma once

#include "Currency.h"


namespace lmh {

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



	// Inline definitions
	inline Currency ExchangeRate::xxx() const { return xxx_; }
	inline Currency ExchangeRate::yyy() const { return yyy_; }
	inline double ExchangeRate::value() const { return value_; }
}
