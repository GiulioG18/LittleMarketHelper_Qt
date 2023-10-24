

// ========================================================================
//		Financial portfolio
// ========================================================================

#pragma once

#include <memory>
#include <set>
#include <map>

#include "Utils/StatusCode.h"
#include "Balance.h"
#include "Cash.h"
#include "WeightedSecurity.h"


namespace lmh {

	class Portfolio
	{
	public:

		using CashSet = std::set<Cash, Cash::Comp>;
		using SecurityPtrSet = std::set<std::shared_ptr<WSecurity>, WSecurity::Comp>;

	public:

		Portfolio(Balance::Ccy ccy);

		// Non-const methods
		Status addCash(const Cash& cash);
		Status removeCash(Currency::Type ccy);
		Status addSecurity(const Security& security);
		Status removeSecurity(const std::string& isin);
		Status reset(Balance::Ccy ccy);
		/*template<EditTrade::Type editType, typename Type>
		Status edit(const std::string& isin, Type newValue);*/

		// Const methods
		size_t size() const;
		Price value() const;

		// Getters
		inline const std::shared_ptr<Balance>& openPosition() const;
		inline const CashSet& cash() const;
		inline const SecurityPtrSet& securities() const;

	private:

		// Check that the openPosition is updated with the portfolio securities, and
		// that the sum of weights equals one
		Status validateSecurities() const;

	private:

		std::shared_ptr<Balance> openPosition_;
		CashSet cash_;
		SecurityPtrSet securities_;
	};


	// Inline definitions
	inline const std::shared_ptr<Balance>& Portfolio::openPosition() const { return openPosition_; }
	inline const Portfolio::CashSet& Portfolio::cash() const { return cash_; }
	inline const Portfolio::SecurityPtrSet& Portfolio::securities() const { return securities_; }


	// Template definitions
	// TODO: simplify this and take it out of here...
	//template<EditTrade::Type editType, typename Type>
	//Status Portfolio::edit(const std::string& isin, Type newValue)
	//{
	//	// set() functions may throw...
	//	try
	//	{
	//		// Check the edit is valid before extracting the trade
	//		if (!EditTrade::validateEditType<editType>(newValue))
	//			FAIL("invalid edit");

	//		// Find trade...
	//		auto trade = trades_->find(isin);
	//		if (trade == trades_->get().end())
	//			return Status::TRADE_NOT_FOUND;

	//		// ...and edit it
	//		auto& security = trade->first;
	//		if		constexpr (editType == EditTrade::NAME)
	//			security->setName(newValue);
	//		else if constexpr (editType == EditTrade::QUANTITY)
	//			security->setQuantity(newValue);
	//		else if constexpr (editType == EditTrade::PRICE)
	//			security->setQuote(newValue);
	//		else
	//			ASSERT(false, "Invalid Edit operation");
	//	}
	//	catch (...)
	//	{
	//		return Status::INVALID_INPUT;
	//	}		

	//	return Status::SUCCESS;
	//}

}
