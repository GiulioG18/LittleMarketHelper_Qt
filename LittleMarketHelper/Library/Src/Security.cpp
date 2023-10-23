
#include "Security.h"
#include "Utils/Assertions.h"


namespace lmh {

	Security::Security(
		const std::string& isin,
		const std::string& name,
		int quantity,
		const Quote& quote
	)
		: 
		isin_(isin), 
		name_(name),
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

	bool Security::validateIsin(const std::string& isin)
	{
		// Could be improved, but for now this is enough
		return isin.length() == 12;
	}

	bool Security::validateQuantity(int quantity)
	{
		return quantity >= 0;
	}

	inline void Security::setName(const std::string& name)
	{
		if (name_ != name)
		{
			name_ = name;
			notifyObservers();
		}
			
	}

	void Security::setQuantity(int quantity)
	{
		REQUIRE(validateQuantity(quantity), "invalid quantity as input");
		if (quantity_ != quantity)
		{
			quantity_ = quantity;
			notifyObservers();
		}			
	}

	void Security::setQuote(double value)
	{
		// Validation is delegated
		if (value != quote_.price().value())
		{
			quote_.setValue(value);
			notifyObservers();
		}
	}
}