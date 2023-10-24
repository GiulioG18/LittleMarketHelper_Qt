
#include <functional>

#include "Price.h"
#include "ExchangeRate.h"


namespace lmh {

	// Helper function for binary operators among Price objects
	template<typename Op>
	Price binaryPriceOp(const Price& first, const Price& second)
	{
		Op op;
		double v;

		if (first.ccy() == second.ccy())
		{
			v = op(first.value(), second.value());
		}
		else
		{
			// Translate the second price into the first currency
			Price secondConverted = ExchangeRateRepository::get().convertPrice(second, first.ccy());
			v = op(first.value(), secondConverted.value());
		}

		ENSURE(v >= 0, "invalid value");
		return { first.ccy(), v };
	}

	// Helper function for binary operators with scalars
	template<typename Op>
	Price binaryDoubleOp(const Price& price, double d)
	{
		REQUIRE(d >= 0, "invalid amount");

		Op op;
		return { price.ccy(), op(price.value(), d) };
	}

	Price::Price(Ccy ccy, double value)
		: ccy_(ccy), value_(value)
	{
		REQUIRE(value_ >= 0, "invalid value");
		// TODO: REQUIRE that the ccy is available
	}

	void Price::set(double amount)
	{
		REQUIRE(amount >= 0, "invalid amount");

		if (value_ != amount)
			value_ = amount;
	}

	// Operators [ MAY THROW ]
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
		if (this->ccy_ == other.ccy_)
		{
			this->value_ += other.value_;
		}
		else
		{
			// Translate the second price into the first currency
			/*double eq = ExchangeRateRepository::convertPrice(other, this->ccy());
			this->value_ += eq;*/
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