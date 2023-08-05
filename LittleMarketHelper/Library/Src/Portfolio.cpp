
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

	bool Portfolio::addTrade(const std::shared_ptr<FinProduct>& product)
	{
		return iTradeset_->add(std::make_pair(
			product, 
			std::make_unique<ActualWeight>(product, balance_))
		);
	}

	bool Portfolio::removeTrade(const std::string& name)
	{
		return iTradeset_->remove(name);
	}

	void Portfolio::excludeTrade()
	{
		// TODO: implement
	}

	void Portfolio::includeTrade()
	{
		// TODO: implement
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