
#include "Security.h"
#include "Utils/Assertions.h"


namespace lmh {

	Security::Security(
		const std::string& isin,
		const std::string& name,
		Currency ccy,
		int quantity,
		float price
	)
		: 
		isin_(isin), 
		name_(name),
		ccy_(ccy),
		quantity_(quantity),
		price_(price)
	{
		REQUIRE(Security::validateIsin(isin_), "invalid isin as input");
		REQUIRE(Security::validateQuantity(quantity_), "invalid quantity as input");
		REQUIRE(Security::validatePrice(price_), "invalid price as input");
	}

	float lmh::Security::openPosition() const
	{
		return static_cast<float>(quantity_) * price_;		
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

	bool Security::validatePrice(float price)
	{
		return price > 0;
	}

	inline void Security::setName(const std::string& name)
	{
		name_ = name;
		notifyObservers();
	}

	void Security::setCcy(Currency ccy)
	{
		ccy_ = ccy;
		notifyObservers();
	}

	void Security::setQuantity(int quantity)
	{
		REQUIRE(validateQuantity(quantity), "invalid quantity as input");
		quantity_ = quantity;
		notifyObservers();
	}

	void Security::setPrice(float price)
	{
		REQUIRE(validatePrice(price), "invalid price as input");
		price_ = price;
		notifyObservers();
	}
}