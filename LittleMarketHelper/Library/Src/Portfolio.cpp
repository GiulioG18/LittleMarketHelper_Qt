
#include <optional>

#include "Portfolio.h"
#include "Utils/Assertions.h"
#include "Weight.h"


namespace lmh {
	

	Portfolio::Portfolio(Currency ccy)
		:
		ccy_(ccy),
		trades_(std::make_shared<Tradeset>()),
		balance_(std::make_shared<Balance>(ccy_, trades_))
	{
	}

	CODE Portfolio::add(const std::shared_ptr<Security>& security)
	{
		// For now only unique ccy is supported inside a portfolio
		if (security->ccy() != this->ccy_)
			return CODE::CURRENCY_NOT_ALLOWED;

		return trades_->insert(std::make_pair(
			security,
			std::make_unique<Weight>(security, balance_))
		);
	}

	CODE Portfolio::remove(const std::string& isin)
	{
		return trades_->erase(isin);
	}

	CODE Portfolio::edit(const std::string& isin, EditTrade::Type t, auto newValue)
	{
		// Check the edit is valid before extracting the trade
		if (!EditTrade::validateEdit(t, newValue))
			return CODE::INVALID_EDIT;

		// Find trade...
		auto trade = trades_->find(isin);
		if (trade == trades_->get().end())
			return CODE::TRADE_NOT_FOUND;

		// ...and edit it
		Security& security = trade->first;
		switch (t)
		{
		case lmh::EditTrade::NAME:		security.setName(newValue);			break;
		case lmh::EditTrade::CURRENCY: 
		{
			// For now only unique ccy is supported inside a portfolio
			if (security.ccy() != this->ccy_)
				return false; 
			security.setCcy(newValue);
			break;
		}
		case lmh::EditTrade::QUANTITY:	security.setQuantity(newValue);		break;
		case lmh::EditTrade::PRICE:		security.setPrice(newValue);		break;

		default: 						FAIL("Invalid logic");				break;
		}		

		return CODE::SUCCESS;
	}

	void Portfolio::clear()
	{
		trades_->clear();
		balance_->clear();
	}

	size_t Portfolio::size() const
	{
		return trades_->size();
	}

}