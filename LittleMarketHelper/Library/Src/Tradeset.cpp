
#include "Tradeset.h"
#include "Utils/Assertions.h"
#include "Weight.h"


namespace lmh {

	void Tradeset::update()
	{
		notifyObservers();
	}

	LmhStatus Tradeset::insert(Trade&& trade)
	{
		auto result = trades_.insert(std::move(trade));
		if (result.second)
		{
			registerWith((result.first)->first);
			notifyObservers();

			return LmhStatus::SUCCESS;
		}

		return LmhStatus::TRADE_NOT_FOUND;
	}

	LmhStatus Tradeset::erase(const std::string& isin)
	{
		Iterator trade = find(isin);
		bool found = (trade != trades_.end());
		if (found)
		{
			unregisterWith(trade->first);
			trades_.erase(trade);
			notifyObservers();

			return LmhStatus::SUCCESS;
		}

		return LmhStatus::TRADE_NOT_FOUND;
	}

	std::optional<Tradeset::Trade> Tradeset::extract(const std::string& isin)
	{
		Iterator it = find(isin);
		if (it == trades_.end())
			return std::nullopt;

		unregisterWith(it->first);
		Trade trade = std::move(trades_.extract(it).value());
		// Delete the Weight since the trade does not belong to a portfolio anymore
		if (trade.second)
			trade.second.reset();
		notifyObservers();

		return trade;
	}

	void Tradeset::clear()
	{
		if (trades_.empty())
			return;

		trades_.clear();
		// Unregister with observed trades (now cleared from set)
		unregisterWithAll();
		notifyObservers();
	}

	size_t Tradeset::size() const
	{
		return trades_.size();
	}

	Tradeset::Iterator Tradeset::find(const std::string& isin) const
	{
		return std::find_if(std::begin(trades_), std::end(trades_),
			[&isin](const Tradeset::Trade& trade)
			{
				return trade.first->isin() == isin;
			}
		);
	}

	bool operator<(const Tradeset::Trade& left, const Tradeset::Trade& right)
	{
		return left.first->isin() < right.first->isin();
	}
}