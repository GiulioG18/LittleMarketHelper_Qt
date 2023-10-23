// ========================================================================
//		Cash
// ========================================================================

#pragma once

#include "Patterns/Observable.h"
#include "Currency.h"

// TODO: move implementation outside


namespace lmh {

	class Cash : public Observable
	{
	public:

		using Ccy = Currency::Type;

		// This comparison functor serves to not allow multiple cash objects with same 
		// currency but different values inside a set
		struct Comp
		{
			bool operator()(const std::shared_ptr<Cash>& first, const std::shared_ptr<Cash>& second) const
			{
				return first->price_.ccy() < second->price_.ccy();
			}
		};

		// Getters
		inline const Price& price() { return price_; }

	private:

		Price price_;
	};

}