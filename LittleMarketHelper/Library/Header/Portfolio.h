

// ========================================================================
//		Financial portfolio
// ========================================================================

#pragma once

#include <memory>

#include "Utils/StatusCode.h"
#include "PortfolioUtilities.h"
#include "Tradeset.h"
#include "Balance.h"
#include "Security.h"


namespace lmh {

	class Portfolio
	{
	public:

		Portfolio(Currency ccy);

		// Non-const methods
		LmhStatus add(const std::shared_ptr<Security>& security);
		LmhStatus remove(const std::string& isin);
		LmhStatus edit(const std::string& isin, EditTrade::Type t, auto newValue);
		void clear();

		// Const methods
		size_t size() const;

		// Getters
		inline Currency ccy() const;
		inline const std::shared_ptr<Tradeset>& trades() const;
		inline const std::shared_ptr<Balance>& balance() const;

	private:

		struct Settings
		{
			bool multiCcy_ = false;
		};

		Currency ccy_;
		std::shared_ptr<Tradeset> trades_;
		std::shared_ptr<Balance> balance_;
		Settings settings_;
	};

	



	// Inline definitions
	inline Currency Portfolio::ccy() const { return ccy_; };
	inline const std::shared_ptr<Tradeset>& Portfolio::trades() const { return trades_; }
	inline const std::shared_ptr<Balance>& Portfolio::balance() const { return balance_; }

}
