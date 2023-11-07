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
		
		Price(Currency currency, double value); // [ MAY THROW ]

		// Getters
		inline double value() const;
		inline Currency currency() const;

		// Setters
		void set(double amount); // [ MAY THROW ]

		// Operators
		Price operator+(const Price& other) const; // [ MAY THROW ]
		Price operator/(const Price& other) const; // [ MAY THROW ]
		Price operator*(const Price& other) const; // [ MAY THROW ]
		Price operator+(double amount) const; // [ MAY THROW ]
		Price operator/(double amount) const; // [ MAY THROW ]
		Price operator*(int quantity) const; // [ MAY THROW ]
		Price& operator+=(const Price& other); // [ MAY THROW ]
		Price& operator+=(double amount); // [ MAY THROW ]

	private:

		Currency currency_;
		double value_;
	};


	// Inline definitions
	inline double Price::value() const { return value_; }
	inline Currency Price::currency() const { return currency_; }

}
