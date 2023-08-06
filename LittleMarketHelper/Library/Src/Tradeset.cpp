
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
		Iterator trade = find(name);
		bool found = (trade != trades_.end());
		if (found)
		{
			unregisterWith(trade->first);
			trades_.erase(trade);
			notifyObservers();
		}

		return found;
	}

	std::optional<Tradeset::Trade> Tradeset::move(const std::string& name)
	{
		Iterator it = find(name);
		if (it == trades_.end())
			return std::nullopt;

		unregisterWith(it->first);
		std::optional<Tradeset::Trade> trade = std::move(trades_.extract(it).value());
		notifyObservers();

		return trade;
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

	Tradeset::Iterator Tradeset::find(const std::string& name) const
	{
		return std::find_if(std::begin(trades_), std::end(trades_),
			[&name](const Tradeset::Trade& trade)
			{
				return trade.first->name() == name;
			}
		);
	}

	bool operator<(const Tradeset::Trade& left, const Tradeset::Trade& right)
	{
		return left.first->name() < right.first->name();
	}
}