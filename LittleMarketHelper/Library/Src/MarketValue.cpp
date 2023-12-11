
#include <cassert>

#include "MarketValue.h"
#include "Utils/InputValidator.h"
#include "Utils/Assertions.h"


namespace lmh {

	MarketValue::MarketValue(Currency currency)
		: price_(0.0), currency_(currency)
	{
		ENSURE(ValidateInput::marketValue(*this), "invalid market value");;
	}


	MarketValue::MarketValue(double value, Currency currency)
		: price_(value), currency_(currency)
	{
		ENSURE(ValidateInput::marketValue(*this), "invalid market value");
	}

	void MarketValue::set(double price)
	{
		price_ = price;
		ENSURE(ValidateInput::marketValue(*this), "invalid market value");
	}


	
	// Operators

	MarketValue MarketValue::operator+(const MarketValue& other) const
	{
		assert(this->currency_ == other.currency_);
		return {this->price_ + other.price_, currency_};
	}

	MarketValue MarketValue::operator*(const MarketValue& other) const
	{
		assert(this->currency_ == other.currency_);
		return { this->price_  * other.price_, currency_ };
	}

	MarketValue MarketValue::operator/(const MarketValue& other) const
	{
		assert(this->currency_ == other.currency_);
		return { this->price_ / other.price_, currency_ };
	}

	MarketValue& MarketValue::operator+=(const MarketValue& other)
	{
		assert(this->currency_ == other.currency_);
		this->price_ += other.price_;
		return *this;
	}

	MarketValue& MarketValue::operator*=(const MarketValue& other)
	{
		assert(this->currency_ == other.currency_);
		this->price_ *= other.price_;
		return *this;
	}

	MarketValue& MarketValue::operator/=(const MarketValue& other)
	{
		assert(this->currency_ == other.currency_);
		this->price_ /= other.price_;
		return *this;
	}



}