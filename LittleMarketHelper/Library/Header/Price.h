// ========================================================================
//		Value - Currency pair
// ========================================================================

#pragma once

#include "Utils/Assertions.h"
#include "Currency.h"

// TODO: move definitions

namespace lmh {

	class Price
	{
	public:

		using Ccy = Currency::Type;

	public:

		// Constructor [ MAY THROW ]
		Price(Ccy ccy, double value)
			: ccy_(ccy), value_(value)
		{
			ENSURE(value_ >= 0, "invalid value");
		}

		// Getters
		double value() const { return value_; }
		Ccy ccy() const { return ccy_; }

		// Setters [ MAY THROW ]
		void set(double amount)
		{
			REQUIRE(amount >= 0, "invalid amount");

			if (value_ != amount)
				value_ = amount;
		}

		// Operators [ MAY THROW ]
		Price operator+(const Price& other) const
		{
			double v;

			if (this->ccy_ == other.ccy_)
			{
				v = this->value_ + other.value_;
			}
			else
			{
				// TODO: 
				// Check connectivity,
				// Convert other to this
				// Multi-currency not allowed so far
				FAIL("Multi-currency not allowed");
			}

			return { this->ccy_, v };
		}

		Price operator/(const Price& other) const
		{
			double v;

			if (this->ccy_ == other.ccy_)
			{
				v = this->value_ / other.value_;
			}
			else
			{
				// TODO: 
				// Check connectivity,
				// Convert other to this
				// Multi-currency not allowed so far
				FAIL("Multi-currency not allowed");
			}

			return { this->ccy_, v };
		}

		Price& operator+=(const Price& other)
		{
			if (this->ccy_ == other.ccy_)
			{
				this->value_ += other.value_;
			}
			else
			{
				// TODO: 
				// Check connectivity,
				// Convert other to this
				// Multi-currency not allowed so far
				FAIL("Multi-currency not allowed");
			}

			return *this;
		}

		Price operator+(double amount) const
		{
			REQUIRE(amount >= 0, "invalid amount");
			return { this->ccy_, this->value_ + amount };
		}

		Price operator/(double amount) const
		{
			REQUIRE(amount >= 0, "invalid amount");
			return { this->ccy_, this->value_ / amount };
		}

		Price operator*(int quantity) const
		{
			REQUIRE(quantity >= 0, "invalid quantity");
			return { this->ccy_, this->value_ * quantity };
		}

		Price& operator+=(double amount)
		{
			REQUIRE(amount >= 0, "invalid amount");
			this->value_ += amount;

			return *this;
		}

	private:

		Ccy ccy_; // Const
		double value_;
	};

}
