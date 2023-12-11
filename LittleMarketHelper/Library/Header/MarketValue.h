
// ========================================================================
//		Market value
// ========================================================================

#pragma once

#include "Currency.h"


namespace lmh {

	class MarketValue
	{
	public:

		MarketValue(Currency currency); // [ MAY THROW ]
		MarketValue(double price, Currency currency); // [ MAY THROW ] 

		// Setters
		void set(double price); // [ MAY THROW ]

		// Getters
		inline double price() const;
		inline Currency currency() const;

		// Operators
		// Market values of different currencies will always be treated as not equal
		auto operator<=>(const MarketValue& other) const = default;
		bool operator==(const MarketValue& other) const = default;
		// NB: Arithmetic operations between 2 market values will call std::abort 
		// when the 2 currencies do not match
		MarketValue operator+(const MarketValue& other) const;
		MarketValue operator*(const MarketValue& other) const;
		MarketValue operator/(const MarketValue& other) const;
		MarketValue& operator+=(const MarketValue& other);
		MarketValue& operator*=(const MarketValue& other);
		MarketValue& operator/=(const MarketValue& other);

	private:

		double price_;
		Currency currency_;
	};



	// Inline definitions
	inline double MarketValue::price() const { return price_; }
	inline Currency MarketValue::currency() const { return currency_; }

}