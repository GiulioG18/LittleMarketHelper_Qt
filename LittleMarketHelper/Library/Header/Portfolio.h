

// ========================================================================
//		Financial portfolio
// ========================================================================

#pragma once

#include <memory>
#include <set>
#include <map>
#include <cassert>

#include "Utils/StatusCode.h"
#include "Balance.h"
#include "Cash.h"
#include "WeightedSecurity.h"
#include "PortfolioUtilities.h"


namespace lmh {

	class Portfolio
	{
	public:

		using CashSet = std::set<Cash, Cash::Comp>;
		using SecurityPtrSet = std::set<std::shared_ptr<WSecurity>, WSecurity::Comp>;

	public:

		Portfolio(Currency currency);

		// Non-const methods
		Status addCash(const Cash& cash);
		Status removeCash(Currency currency);
		Status addSecurity(const Security& security);
		Status removeSecurity(std::string_view isin);
		Status reset(Currency currency);
		template<Edit::Type et, typename Type>
		Status edit(std::string_view isin, Type newValue);

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

	template<Edit::Type et, typename Type>
	Status Portfolio::edit(std::string_view isin, Type newValue)
	{
		try
		{
			// Check the edit is valid before extracting the trade
			if (!Edit::validateEditType<et>(newValue))
				FAIL("invalid edit");

			// Find trade...
			auto security = securities_.find(isin);
			if (security == securities_.end())
				return Status::TRADE_NOT_FOUND;

			// ...and edit it
			if		constexpr (et == Edit::NAME)
				security->get()->setName(newValue);
			else if constexpr (et == Edit::QUANTITY)
				security->get()->setQuantity(newValue);
			else if constexpr (et == Edit::PRICE)
				security->get()->setQuote(newValue);
			else
				assert(false && "invalid edit");
		}
		catch (...)
		{
			return Status::INVALID_INPUT;
		}		

		return Status::SUCCESS;
	}

}
