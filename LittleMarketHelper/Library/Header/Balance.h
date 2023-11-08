// ========================================================================
//		Generic balance
// ========================================================================

#pragma once

#include <memory>
#include <string_view>

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

		using SecurityPtrSet = std::set<std::shared_ptr<WSecurity>, WSecurity::Comp>;

	public:

		Balance(Currency Currency);

		// Non-const methods
		void update() override;

		// Const methods
		Status registerSecurity(std::shared_ptr<WSecurity> security);
		Status unregisterSecurity(std::string_view isin);

		// Getters
		inline const Price& price() const;
		inline const SecurityPtrSet& securities() const;

	private:

		void computeBalance();

	private:

		Price price_;
		SecurityPtrSet securities_; // Observed
	};


	// Inline definitions
	inline const Price& Balance::price() const { return price_; }
	inline const Balance::SecurityPtrSet& Balance::securities() const { return securities_; }

}
