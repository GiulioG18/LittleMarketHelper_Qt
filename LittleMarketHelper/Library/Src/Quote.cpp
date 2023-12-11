
#include <cassert>

#include "Quote.h"
#include "Utils/Assertions.h"
#include "Utils/InputValidator.h"
#include "MarketData/Isin.h"

/*
// Helper function for binary operators between 2 quotes
template<typename Op>
Quote binaryQuoteOp(const Quote& first, const Quote& second)
{
	Op op;
	double translatedprice = 0.0;

	if (first.currency() == second.currency())
	{
		translatedprice = op(first.price(), second.price());
	}
	else
	{
		// Translate the second quote price into the first currency
		Quote secondConverted = Forex::convertQuote(second, first.currency());
		translatedprice = op(first.price(), secondConverted.price());
	}

	ENSURE(translatedprice >= 0, "invalid value");

	return { translatedprice, first.currency() };
}

// Helper function for binary operators with scalars (currency is ignored)
template<typename Op>
Quote binaryScalarOp(const Quote& Quote, double scalar)
{
	REQUIRE(ValidateInput::price(scalar), "invalid price as input");

	Op op;

	return { Quote.currency(), op(Quote.price(), scalar) };
}

Quote::Quote(double price, Currency currency)
	: price_(price), currency_(currency)
{
	REQUIRE(ValidateInput::price(price), "invalid price as input");
	REQUIRE(ValidateInput::currency(currency), "currency is not available");
}

void Quote::set(double price)
{
	REQUIRE(ValidateInput::price(price), "invalid price as input");
	if (price_ != price)
		price_ = price;
}

bool Quote::operator==(const Quote& other) const
{
	Quote convertedOther = Forex::convertQuote(other, this->currency_);
	return this->price_ == convertedOther.price_;
}

Quote Quote::operator+(const Quote& other) const
{
	return binaryQuoteOp<std::plus<double>>(*this, other);
}

Quote Quote::operator/(const Quote& other) const
{
	return binaryQuoteOp<std::divides<double>>(*this, other);
}

Quote Quote::operator*(const Quote& other) const
{
	return binaryQuoteOp<std::multiplies<double>>(*this, other);
}

Quote Quote::operator+(double price) const
{
	return binaryScalarOp<std::plus<double>>(*this, price);
}

Quote Quote::operator/(double price) const
{
	return binaryScalarOp<std::divides<double>>(*this, price);
}

Quote Quote::operator*(double quantity) const
{
	return binaryScalarOp<std::multiplies<double>>(*this, quantity);
}

Quote& Quote::operator+=(const Quote& other)
{
	*this = binaryQuoteOp<std::plus<double>>(*this, other);
	return *this;
}

Quote& Quote::operator+=(double price)
{
	REQUIRE(price >= 0, "invalid price");

	this->price_ += price;

	return *this;
}
*/

namespace lmh {
	
	Quote::Quote(const MarketValue& userMv, const MarketValue& baseMv, const Date& date)
		: userMv_(userMv), baseMv_(baseMv), date_(date)
	{
		// Validation for quotes is stricter than for market values
		assert(userMv_.price() > 0.0);
		assert(baseMv_.price() > 0.0);
	}
}