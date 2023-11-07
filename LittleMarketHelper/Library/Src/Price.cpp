
#include <functional>

#include "Price.h"
#include "ExchangeRate.h"


namespace lmh {

	// Helper function for binary operators between 2 Prices
	template<typename Op>
	Price binaryPriceOp(const Price& first, const Price& second)
	{
		Op op;
		double v;

		if (first.currency() == second.currency())
		{
			v = op(first.value(), second.value());
		}
		else
		{
			// Translate the second price into the first currency
			Price secondConverted = Forex::convert(second, first.currency());
			v = op(first.value(), secondConverted.value());
		}

		ENSURE(v >= 0, "invalid value");
		return { first.currency(), v };
	}

	// Helper function for binary operators with scalars
	template<typename Op>
	Price binaryDoubleOp(const Price& price, double d)
	{
		REQUIRE(d >= 0, "invalid amount");

		Op op;
		return { price.currency(), op(price.value(), d) };
	}


	// Price

	Price::Price(Currency currency, double value)
		: currency_(currency), value_(value)
	{
		REQUIRE(value_ >= 0, "invalid value");
		REQUIRE(Forex::availableCurrency(currency), "currency is not available");
	}

	void Price::set(double amount)
	{
		REQUIRE(amount >= 0, "invalid amount");

		if (value_ != amount)
			value_ = amount;
	}

	Price Price::operator+(const Price& other) const
	{
		return binaryPriceOp<std::plus<double>>(*this, other);
	}

	Price Price::operator/(const Price& other) const
	{
		return binaryPriceOp<std::divides<double>>(*this, other);
	}

	Price Price::operator*(const Price& other) const
	{
		return binaryPriceOp<std::multiplies<double>>(*this, other);
	}

	Price Price::operator+(double amount) const
	{
		return binaryDoubleOp<std::plus<double>>(*this, amount);
	}

	Price Price::operator/(double amount) const
	{
		return binaryDoubleOp<std::divides<double>>(*this, amount);
	}

	Price Price::operator*(int quantity) const
	{
		return binaryDoubleOp<std::multiplies<double>>(*this, static_cast<double>(quantity));
	}

	Price& Price::operator+=(const Price& other)
	{
		// TODO: modify the Price-Price binary fun helper to be used also in these operators
		if (this->currency_ == other.currency_)
		{
			this->value_ += other.value_;
		}
		else
		{
			// Translate the other price into this price's currency			
			Price otherConverted = Forex::convert(other, this->currency());
			this->value_ += otherConverted.value();
		}

		ENSURE(this->value_ >= 0, "invalid value");
		return *this;
	}

	Price& Price::operator+=(double amount)
	{
		REQUIRE(amount >= 0, "invalid amount");
		this->value_ += amount;

		return *this;
	}

}