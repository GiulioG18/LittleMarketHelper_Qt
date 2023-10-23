
#include "WeightedSecurity.h"


namespace lmh {

	WSecurity::WSecurity(const Security& security, const std::shared_ptr<Balance>& op)
		: Security(security), weight_(this, op)
	{
	}
	
	bool WSecurity::Comp::operator()(const std::shared_ptr<WSecurity> first, const std::shared_ptr<WSecurity>& second) const
	{
		return first->isin() < second->isin();
	}

	bool WSecurity::Comp::operator()(const std::string& first, const std::shared_ptr<WSecurity>& second) const
	{
		return first < second->isin();
	}

	bool WSecurity::Comp::operator()(const std::shared_ptr<WSecurity>& first, const std::string& second) const
	{
		return first->isin() < second;
	}



	//void Tradeset::update()
	//{
	//	notifyObservers();
	//}

	//Status Tradeset::insert(Trade&& trade)
	//{
	//	auto result = trades_.insert(std::move(trade));
	//	if (!result.second)
	//		return Status::TRADE_DUPLICATE_NOT_INSERTED;

	//	registerWith((result.first)->first); // Register with the security
	//	notifyObservers();

	//	return Status::SUCCESS;
	//}

	//Status Tradeset::erase(const std::string& isin)
	//{
	//	Iterator trade = find(isin);
	//	if (trade == trades_.end())
	//		return Status::TRADE_NOT_FOUND;
	//	
	//	unregisterWith(trade->first);
	//	trades_.erase(trade);
	//	notifyObservers();

	//	return Status::SUCCESS;
	//}

	//void Tradeset::clear()
	//{
	//	if (trades_.empty())
	//		return;

	//	trades_.clear();
	//	// Unregister with observed trades (now cleared from set)
	//	unregisterWithAll();
	//	notifyObservers();
	//}

	//size_t Tradeset::size() const
	//{
	//	return trades_.size();
	//}

	//Tradeset::Iterator Tradeset::find(const std::string& isin) const
	//{
	//	return std::find_if(std::begin(trades_), std::end(trades_),
	//		[&isin](const Tradeset::Trade& trade)
	//		{
	//			return trade.first->isin() == isin;
	//		}
	//	);
	//}

	//bool operator<(const Tradeset::Trade& left, const Tradeset::Trade& right)
	//{
	//	return left.first->isin() < right.first->isin();
	//}
}