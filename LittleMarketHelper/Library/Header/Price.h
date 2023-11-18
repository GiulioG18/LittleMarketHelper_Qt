// ========================================================================
//		Value - Currency pair
// ========================================================================

#pragma once

#include "Utils/Assertions.h"
#include "Currency.h"
#include "Forex.h"


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
		inline auto operator<=>(const Price& other) const;
		bool operator==(const Price& other) const;
		Price operator+(const Price& other) const; 
		Price operator/(const Price& other) const; 
		Price operator*(const Price& other) const; 
		Price& operator+=(const Price& other); 
		Price operator+(double amount) const; 
		Price operator/(double amount) const; 
		Price operator*(double quantity) const;
		Price& operator+=(double amount);

	private:

		Currency currency_;
		double amount_;
	};


	// Inline definitions
	inline double Price::amount() const { return amount_; }
	inline Currency Price::currency() const { return currency_; }


	// Template definitions

	inline auto Price::operator<=>(const Price& other) const
	{
		Price convertedOther = Forex::convert(other, this->currency_);
		return this->amount_ <=> convertedOther.amount_;
	}

}
