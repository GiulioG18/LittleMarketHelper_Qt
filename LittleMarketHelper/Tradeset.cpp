
#include "Tradeset.h"
#include "Assertions.h"
#include "Weight.h"


namespace lmh {

	void Tradeset::update()
	{
		notifyObservers();
	}

	void Tradeset::add(Trade&& trade)
	{
		registerWith(trade.first);
		bool inserted = trades_.insert(std::move(trade)).second;
		VALIDATE_USER_INPUT(inserted, "adding a duplicate product");

		notifyObservers();
	}

	void Tradeset::remove()
	{
		notifyObservers();
	}

	size_t Tradeset::size() const
	{
		return trades_.size();
	}

	bool operator<(const Tradeset::Trade& left, const Tradeset::Trade& right)
	{
		return left.first->name() < right.first->name();
	}

}