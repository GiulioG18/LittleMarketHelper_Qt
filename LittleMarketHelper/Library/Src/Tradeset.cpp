
#include "Tradeset.h"
#include "Utils/Assertions.h"
#include "Weight.h"


namespace lmh {

	void Tradeset::update()
	{
		notifyObservers();
	}

	bool Tradeset::add(Trade&& trade)
	{
		bool inserted = trades_.insert(std::move(trade)).second;
		if (inserted)
		{
			registerWith(trade.first);
			notifyObservers();
		}

		return inserted;
	}

	bool Tradeset::remove(const std::string& name)
	{
		auto it = std::find_if(std::begin(trades_), std::end(trades_),
			[&name](const Tradeset::Trade& trade) 
			{ 
				return trade.first->name() == name; 
			}
		);

		bool found = it != trades_.end();
		if (found)
		{
			unregisterWith(it->first);
			trades_.erase(it);
			notifyObservers();
		}

		return found;
	}

	void Tradeset::clear()
	{
		if (trades_.empty())
			return;

		trades_.clear();
		// Unregister with observed products (now cleared from set)
		unregisterWithAll();
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