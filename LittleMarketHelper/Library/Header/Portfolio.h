
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
		bool add(const std::shared_ptr<FinProduct>& product);
		bool remove(const std::string& name);
		// Move trade from iTradeset to eTradeset
		// NB: it could successfully extract trade
		// but fail to insert it into second tradeset
		bool exclude(const std::string& name);	
		// Move trade from eTradeset to iTradeset
		// NB: it could successfully extract trade 
		// but fail to insert it into second tradeset
		bool include(const std::string& name);
		void clear();

		// Const methods
		size_t size() const;
		size_t excludedTradesCount() const;
		inline const std::shared_ptr<Tradeset>& included() const;
		inline const std::shared_ptr<Tradeset>& excluded() const;
		inline const std::shared_ptr<Balance>& balance() const;
		
	private:

		std::shared_ptr<Tradeset> iTradeset_;		// Included trades
		std::shared_ptr<Tradeset> eTradeset_;		// Excluded trades (no weight)
		std::shared_ptr<Balance> balance_;
	};


	// Inline definitions

	inline const std::shared_ptr<Tradeset>& Portfolio::included() const { return iTradeset_; }
	inline const std::shared_ptr<Tradeset>& Portfolio::excluded() const { return eTradeset_; }
	inline const std::shared_ptr<Balance>& Portfolio::balance() const { return balance_; }
}
