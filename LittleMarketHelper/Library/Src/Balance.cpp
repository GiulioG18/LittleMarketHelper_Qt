
#include <algorithm>

#include "Balance.h"
#include "Utils/Assertions.h"
#include "WeightedSecurity.h"


namespace lmh {

	Balance::Balance(Currency currency)
		: price_(currency, 0.0)
	{	
	}

	void Balance::update()
	{
		computeBalance(); // Calls notifyObservers();
	}

	Status lmh::Balance::registerSecurity(std::shared_ptr<WSecurity> security)
	{
		REQUIRE(security, "invalid security");

		auto insertion = securities_.insert(std::move(security));
		if (!insertion.second)
			return Status::TRADE_DUPLICATE_NOT_INSERTED;

		registerWith(*insertion.first);
		computeBalance(); // Calls notifyObservers();

		return Status::SUCCESS;
	}

	Status lmh::Balance::unregisterSecurity(std::string_view isin)
	{
		auto it = securities_.find(isin);
		if (it == securities_.end())
			return Status::TRADE_NOT_FOUND;

		unregisterWith(*it);
		securities_.erase(it);		
		computeBalance(); // Calls notifyObservers();

		return Status::SUCCESS;
	}

	void Balance::computeBalance()
	{
		price_.set(0.0);
		std::for_each(std::begin(securities_), std::end(securities_),
			[this](auto& security)
			{
				ASSERT(security, "invalid security");
				price_ += security->marketValue();
			}
		);

		notifyObservers();
	}
}