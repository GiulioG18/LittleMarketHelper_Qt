// ========================================================================
//		Generic balance
// ========================================================================

// TODO: DELETE



//
//#pragma once
//
//#include "Patterns/Observable.h"
//#include "Utils/TypeTraits.h"
//#include "Currency.h"
//#include "MarketData/DatasetAccess.h"
//
///*
//#include <memory>
//#include <string_view>
//
//#include "Patterns/Observable.h"
//#include "Utils/StatusCode.h"
//#include "Currency.h"
//#include "WeightedSecurity.h"
//#include "Price.h"
//
//
//namespace lmh {
//
//
//	class Balance : 
//		public Observable,
//		public Observer
//	{
//	public:
//
//		using SecurityPtrSet = std::set<std::shared_ptr<WSecurity>, WSecurity::Comp>;
//
//	public:
//
//		Balance(Currency Currency);
//
//		// Non-const methods
//		void update() override;
//
//		// Const methods
//		Status registerSecurity(std::shared_ptr<WSecurity> security);
//		Status unregisterSecurity(std::string_view isin);
//
//		// Getters
//		inline const Price& price() const;
//		inline const SecurityPtrSet& securities() const;
//
//	private:
//
//		void computeBalance();
//
//	private:
//
//		Price price_;
//		SecurityPtrSet securities_; // Observed
//	};
//
//
//	// Inline definitions
//	inline const Price& Balance::price() const { return price_; }
//	inline const Balance::SecurityPtrSet& Balance::securities() const { return securities_; }
//
//}
//*/
//
//
//namespace lmh {
//
//	// Definition
//
//	template<typename Type>
//	requires IsObservable<Type>// && HasAccumulate<Type> // TODO: check why cant add this requirement for Holdings
//	class Balance : 
//		public Observer, 
//		public Observable
//	{
//	public:
//
//		Balance(
//			Currency currency,
//			const DatasetAccess& dataset,
//			Type observed);
//
//		// Non-const methods
//		virtual void update() override;
//
//	private:
//
//		double value_;
//		Currency currency_;
//		DatasetAccess dataset_;
//		std::shared_ptr<Type> observed_; // Observed
//	};
//
//
//
//
//	// Implementation
//
//	template<typename Type>
//	inline Balance<Type>::Balance(Currency currency, const DatasetAccess& dataset, Type observed)
//		:
//		value_(0.0),
//		currency_(currency),
//		dataset_(dataset),
//		observed_(std::move(observed))
//	{
//	}
//
//	template<typename Type>
//	inline void Balance<Type>::update()
//	{
//		// TODO: impl
//		Quote quote = observed_->accumulate();
//
//		Forex::convertQuote
//
//		notifyObservers();
//	}
//
//}





//
//#include <algorithm>
//
//#include "Balance.h"
//#include "Utils/Assertions.h"
//#include "WeightedSecurity.h"
//
//
//namespace lmh {
//
//	Balance::Balance(Currency currency)
//		: price_(currency, 0.0)
//	{	
//	}
//
//	void Balance::update()
//	{
//		computeBalance(); // Calls notifyObservers();
//	}
//
//	Status lmh::Balance::registerSecurity(std::shared_ptr<WSecurity> security)
//	{
//		REQUIRE(security, "invalid security");
//
//		auto insertion = securities_.insert(std::move(security));
//		if (!insertion.second)
//			return Status::TRADE_DUPLICATE_NOT_INSERTED;
//
//		registerWith(*insertion.first);
//		computeBalance(); // Calls notifyObservers();
//
//		return Status::SUCCESS;
//	}
//
//	Status lmh::Balance::unregisterSecurity(std::string_view isin)
//	{
//		auto it = securities_.find(isin);
//		if (it == securities_.end())
//			return Status::TRADE_NOT_FOUND;
//
//		unregisterWith(*it);
//		securities_.erase(it);		
//		computeBalance(); // Calls notifyObservers();
//
//		return Status::SUCCESS;
//	}
//
//	void Balance::computeBalance()
//	{
//		price_.set(0.0);
//		std::for_each(std::begin(securities_), std::end(securities_),
//			[this](auto& security)
//			{
//				ASSERT(security, "invalid security");
//				price_ += security->marketValue();
//			}
//		);
//
//		notifyObservers();
//	}
//}
