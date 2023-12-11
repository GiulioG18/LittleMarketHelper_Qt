
#include "MarketData/Isin.h"
#include "Utils/Assertions.h"
#include "Utils/InputValidator.h"


namespace lmh {

	Isin::Isin(std::string_view isin)
		: value_(isin)
	{
		REQUIRE(ValidateInput::isin(value_), "invalid isin");
	}

	bool Isin::operator<(const Isin& other) const
	{
		return this->value_ < other.value_;
	}

}