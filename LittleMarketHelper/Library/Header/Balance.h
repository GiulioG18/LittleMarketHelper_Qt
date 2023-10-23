// ========================================================================
//		Generic balance
// ========================================================================

#pragma once

#include <memory>

#include "Patterns/Observable.h"
#include "Utils/StatusCode.h"
#include "Currency.h"
#include "WeightedSecurity.h"
#include "Price.h"


namespace lmh {


	class Balance : 
		public Observable,
		public Observer
	{
	public:

		using Ccy = Currency::Type;
		using SecurityPtrSet = std::set<std::shared_ptr<WSecurity>, WSecurity::Comp>;

	public:

		Balance(Ccy ccy);

		// Non-const methods
		void update() override;

		// Const methods
		Status registerSecurity(std::shared_ptr<WSecurity> security);
		Status unregisterSecurity(const std::string& isin);

		// Getters
		inline const Price& price() const;
		inline const SecurityPtrSet& securities() const;

	private:

		Price price_;
		SecurityPtrSet securities_; // Observed
	};


	// Inline definitions
	inline const Price& Balance::price() const { return price_; }
	inline const Balance::SecurityPtrSet& Balance::securities() const { return securities_; }

}
