// ========================================================================
//		Value - Currency pair
// ========================================================================

#pragma once

#include "Utils/Assertions.h"
#include "Currency.h"

// TODO: move definitions
// TODO: naming is a bit inconsistent throughout the whole library...

namespace lmh {

	class Price
	{
	public:

		using Ccy = Currency::Type;

	public:

		// Constructor [ MAY THROW ]
		Price(Ccy ccy, double value);

		// Getters
		inline double value() const;
		inline Ccy ccy() const;

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

		Ccy ccy_; // Const
		double value_;
	};


	// Inline definitions
	inline double Price::value() const { return value_; }
	inline Price::Ccy Price::ccy() const { return ccy_; }

}
