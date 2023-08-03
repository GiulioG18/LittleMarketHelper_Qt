
// ========================================================================
//		Financial portfolio
// ========================================================================

#pragma once

#include <memory>

#include "Tradeset.h"
#include "Balance.h"
#include "FinProduct.h"


namespace lmh {

	class Portfolio
	{
	public:

		Portfolio();		

		// Non-const methods
		void addTrade(const std::shared_ptr<FinProduct>& product);
		void removeTrade(const std::string& name);
		void excludeTrade();		
		void clear();

		// Const methods
		size_t size() const;
		size_t excludedTradesCount() const;
		inline const std::shared_ptr<Tradeset>& iTradeset() const;
		inline const std::shared_ptr<Tradeset>& eTradeset() const;
		inline const std::shared_ptr<Balance>& balance() const;
		
	private:

		std::shared_ptr<Tradeset> iTradeset_;		// Included trades
		std::shared_ptr<Tradeset> eTradeset_;		// Excluded trades (no weight)
		std::shared_ptr<Balance> balance_;
	};


	// Inline definitions

	inline const std::shared_ptr<Tradeset>& Portfolio::iTradeset() const { return iTradeset_; }
	inline const std::shared_ptr<Tradeset>& Portfolio::eTradeset() const { return eTradeset_; }
	inline const std::shared_ptr<Balance>& Portfolio::balance() const { return balance_; }
}
