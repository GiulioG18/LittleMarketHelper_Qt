
#include <optional>

#include "Portfolio.h"
#include "Utils/Assertions.h"
#include "Weight.h"


namespace lmh {
	

	Portfolio::Portfolio()
		:
		iTradeset_(std::make_shared<Tradeset>()),
		eTradeset_(std::make_shared<Tradeset>()),
		balance_(std::make_shared<Balance>(iTradeset_))
	{
	}

	bool Portfolio::add(const std::shared_ptr<FinProduct>& product)
	{
		return iTradeset_->insert(std::make_pair(
			product, 
			std::make_unique<Weight>(product, balance_))
		);
	}

	bool Portfolio::remove(const std::string& name)
	{
		return iTradeset_->erase(name);
	}

	bool Portfolio::exclude(const std::string& name)
	{
		bool status = true;
		auto trade = iTradeset_->extract(name);
		status = trade.has_value();

		if (status)
		{
			// If trade has been excluded, there must be no weight object
			ASSERT(!trade.value().second,
				"weight object found in trade outside of portfolio");
			status = eTradeset_->insert(std::move(trade.value()));
		}

		return status;
	}

	bool Portfolio::include(const std::string& name)
	{
		bool status = true;
		auto trade = eTradeset_->extract(name);
		status = trade.has_value();

		if (status)
		{
			// Create an ActualWeight for the trade
			std::unique_ptr<Weight>& weight = trade.value().second;
			ASSERT(!weight, "there was a weight object inside an excluded trade");
			weight = std::make_unique<Weight>(trade.value().first, balance_);

			status = iTradeset_->insert(std::move(trade.value()));
		}

		return status;
	}

	void Portfolio::clear()
	{
		iTradeset_->clear();
		eTradeset_->clear();
		balance_->clear();
	}

	size_t Portfolio::size() const
	{
		return iTradeset_->size();
	}
	size_t Portfolio::excludedTradesCount() const
	{
		return eTradeset_->size();
	}

}