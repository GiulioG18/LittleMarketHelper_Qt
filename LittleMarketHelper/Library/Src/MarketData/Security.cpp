
#include "MarketData/Security.h"
#include "Utils/Assertions.h"

/*

#include "Utils/Assertions.h"
#include "Security.h"
#include "SecurityShell.h"
#include "Utils/InputValidator.h"

Security::Security(
	std::string_view isin,
	std::string_view name,
	uint32_t quantity,
	const Quote& quote)
	:
	isin_(isin),
	name_(name),
	quantity_(quantity),
	quote_(quote)
{
	REQUIRE(ValidateInput::isin(isin_), "invalid isin as input");
	REQUIRE(ValidateInput::quantity(quantity_), "invalid quantity as input");
}

Security::Security(
	const SecurityShell& shell,
	uint32_t quantity,
	const Quote& quote)
	:
	isin_(shell.isin()),
	name_(shell.name()),
	quantity_(quantity),
	quote_(quote)
{
	REQUIRE(ValidateInput::isin(isin_), "invalid isin as input");
	REQUIRE(ValidateInput::quantity(quantity_), "invalid quantity as input");
}

Price lmh::Security::marketValue() const
{
	return quote_.price() * static_cast<double>(quantity_);
}

void Security::setName(std::string_view name)
{
	if (name_ != name)
	{
		name_ = name;
		notifyObservers();
	}

}

void Security::setQuantity(uint32_t quantity)
{
	REQUIRE(ValidateInput::quantity(quantity), "invalid quantity as input");
	if (quantity_ != quantity)
	{
		quantity_ = quantity;
		notifyObservers();
	}
}

void Security::setQuote(double amount)
{
	REQUIRE(ValidateInput::amount(amount), "invalid amount as input");
	if (amount != quote_.price().amount())
	{
		quote_.setAmount(amount);
		notifyObservers();
	}
}*/


namespace lmh {

	Security::Security(Isin isin)
		: isin_(isin)
	{
		// TODO: impl
	}

	Security::Security(Isin isin, std::string name, std::string description, Symbol symbol, StockExchange stockExchange)
		: isin_(isin), name_(name), description_(description), symbol_(symbol), stockExchange_(stockExchange)
	{
	}

	
	bool Security::operator<(const Security& other) const
	{
		if (this->isin_ < other.isin_)
		{
			ASSERT(
				this->name_ != other.name_ && 
				this->description_ != other.description_, 
				"different ISIN, but same name and description");
		}

		return this->isin_ < other.isin_;
	}
}