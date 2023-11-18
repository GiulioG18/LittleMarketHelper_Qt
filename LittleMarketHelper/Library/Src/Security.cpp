
#include "Security.h"
#include "SecurityShell.h"
#include "Utils/Assertions.h"


namespace lmh {

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
		REQUIRE(Security::validateIsin(isin_), "invalid isin as input");
		REQUIRE(Security::validateQuantity(quantity_), "invalid quantity as input");
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
		REQUIRE(Security::validateIsin(isin_), "invalid isin as input");
		REQUIRE(Security::validateQuantity(quantity_), "invalid quantity as input");
	}

	Price lmh::Security::marketValue() const
	{
		return quote_.price() * static_cast<double>(quantity_);
	}

	bool Security::validateIsin(std::string_view isin)
	{
		// Could be improved, but for now this is enough
		return isin.length() == 12;
	}

	bool Security::validateQuantity(uint32_t quantity)
	{
		return quantity >= 0;
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
		REQUIRE(validateQuantity(quantity), "invalid quantity as input");
		if (quantity_ != quantity)
		{
			quantity_ = quantity;
			notifyObservers();
		}			
	}

	void Security::setQuote(double amount)
	{
		// Validation is delegated
		if (amount != quote_.price().amount())
		{
			quote_.setAmount(amount);
			notifyObservers();
		}
	}
}