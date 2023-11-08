
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
			v = op(first.amount(), second.amount());
		}
		else
		{
			// Translate the second price into the first currency
			Price secondConverted = Forex::convert(second, first.currency());
			v = op(first.amount(), secondConverted.amount());
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
		return { price.currency(), op(price.amount(), d) };
	}


	// Price

	Price::Price(Currency currency, double value)
		: currency_(currency), amount_(value)
	{
		REQUIRE(amount_ >= 0, "invalid value");
		REQUIRE(Forex::availableCurrency(currency), "currency is not available");
	}

	void Price::set(double amount)
	{
		REQUIRE(amount >= 0, "invalid amount");

		if (amount_ != amount)
			amount_ = amount;
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

	Price Price::operator*(double quantity) const
	{
		return binaryDoubleOp<std::multiplies<double>>(*this, quantity);
	}

	Price& Price::operator+=(const Price& other)
	{
		*this = binaryPriceOp<std::plus<double>>(*this, other);
		return *this;
	}

	Price& Price::operator+=(double amount)
	{
		REQUIRE(amount >= 0, "invalid amount");
		this->amount_ += amount;

		return *this;
	}

}
