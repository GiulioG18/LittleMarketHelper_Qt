// ========================================================================
//		Value - Currency pair
// ========================================================================

#pragma once

#include "Utils/Assertions.h"
#include "Currency.h"


namespace lmh {

	class Price
	{
	public:
		
		Price(Currency currency, double value); // [ MAY THROW ]

		// Getters
		inline double amount() const;
		inline Currency currency() const;

		// Setters
		void set(double amount); // [ MAY THROW ]

		// Operators
		Price operator+(const Price& other) const; // [ MAY THROW ]
		Price operator/(const Price& other) const; // [ MAY THROW ]
		Price operator*(const Price& other) const; // [ MAY THROW ]
		Price operator+(double amount) const; // [ MAY THROW ]
		Price operator/(double amount) const; // [ MAY THROW ]
		Price operator*(double quantity) const; // [ MAY THROW ]
		Price& operator+=(const Price& other); // [ MAY THROW ]
		Price& operator+=(double amount); // [ MAY THROW ]

	private:

		Currency currency_;
		double amount_;
	};


	// Inline definitions
	inline double Price::amount() const { return amount_; }
	inline Currency Price::currency() const { return currency_; }

}
