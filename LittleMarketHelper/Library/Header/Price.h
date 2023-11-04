// ========================================================================
//		Value - Currency pair
// ========================================================================

#pragma once

#include "Utils/Assertions.h"
#include "Currency.h"

// TODO: naming is a bit inconsistent throughout the whole library...

namespace lmh {

	class Price
	{
	public:

		// Constructor [ MAY THROW ]
		Price(Currency currency, double value);

		// Getters
		inline double value() const;
		inline Currency currency() const;

		// Setters [ MAY THROW ]
		void set(double amount);

		// Operators [ MAY THROW ]
		Price operator+(const Price& other) const;
		Price operator/(const Price& other) const;
		Price operator*(const Price& other) const;
		Price operator+(double amount) const;
		Price operator/(double amount) const;
		Price operator*(int quantity) const;
		Price& operator+=(const Price& other);
		Price& operator+=(double amount);

	private:

		Currency currency_;
		double value_;
	};


	// Inline definitions
	inline double Price::value() const { return value_; }
	inline Currency Price::currency() const { return currency_; }

}
