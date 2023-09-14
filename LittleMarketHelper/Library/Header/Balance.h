// ========================================================================
//		Portfolio balance
// ========================================================================

#pragma once

#include <memory>

#include "Currency.h"
#include "Patterns/Observable.h"


namespace lmh {

	// Forward declarations
	class Tradeset;


	class Balance : 
		public Observable,
		public Observer
	{
	public:

		Balance(Currency ccy, std::shared_ptr<Tradeset> trades);

		// Non-const methods
		void update() override;
		void clear();

		// Const methods
		inline float value() const;

	private:

		float value_;
		Currency ccy_;		
		std::shared_ptr<Tradeset> trades_;	// Observed
	};


	// Inline definitions

	inline float Balance::value() const { return value_; }

}
