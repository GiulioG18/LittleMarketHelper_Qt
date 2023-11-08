
#include <numeric>
#include <algorithm>

#include "Portfolio.h"
#include "Utils/Assertions.h"
#include "WeightedSecurity.h"


namespace lmh {
	

	Portfolio::Portfolio(Currency currency)
		: openPosition_(std::make_shared<Balance>(currency))
	{
	}

	Status Portfolio::addCash(const Cash& cash)
	{
		auto insertion = cash_.insert(cash);
		if (!insertion.second)
			return Status::CASH_DUPLICATE_NOT_INSERTED;
		
		return Status::SUCCESS;
	}

	Status Portfolio::removeCash(Currency currency)
	{
		auto it = cash_.find(currency);
		if (it == cash_.end())
			return Status::CASH_NOT_FOUND;

		cash_.erase(it);

		return Status::SUCCESS;
	}

	Status Portfolio::addSecurity(const Security& security)
	{
		// Move new weighted security it into the portfolio
		auto insertion = securities_.insert(std::make_shared<WSecurity>(security, openPosition_));
		if (!insertion.second)
			return Status::TRADE_DUPLICATE_NOT_INSERTED;

		// Register the open position with this newly created security
		Status status = openPosition_->registerSecurity(*insertion.first);
		if (status != Status::SUCCESS)
		{
			auto deletion = securities_.erase(insertion.first);
			return status;
		}

		return validateSecurities();
	}

	Status Portfolio::removeSecurity(std::string_view isin)
	{
		auto it = securities_.find(isin);
		if (it == securities_.end())
			return Status::TRADE_NOT_FOUND;
		securities_.erase(it);

		Status status = openPosition_->unregisterSecurity(isin);
		ASSERT(status == Status::SUCCESS, "could not remove security from open position");

		return validateSecurities();
	}

	Status Portfolio::reset(Currency currency)
	{
		openPosition_.reset();
		cash_.clear();
		securities_.clear();		
	
		return Status::SUCCESS;
	}

	size_t Portfolio::size() const
	{
		return securities_.size();
	}

	Price Portfolio::value() const
	{
		// Prudent check
		validateSecurities();
		
		Price p = openPosition_->price();
		for (const auto& cash : cash_)
		{
			p += cash.price();
		}

		return p;
	}

	Status Portfolio::validateSecurities() const
	{
		ASSERT(securities_ == openPosition_->securities(), "security sets are not matching");
		
		// Sum of all security weights is 1.0
		double weights = 0.0;
		std::for_each(std::begin(securities_), std::end(securities_),
			[&weights](auto& security)
			{
				ASSERT(security, "invalid security");
				weights += security->weight().value();
			}
		);
		ASSERT(fabs(weights - 1) < 1.0, "weight sum is not 1.0");

		return Status::SUCCESS;
	}
}