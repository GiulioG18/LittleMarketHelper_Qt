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
		// For now only single currency is supported 

	public:

		Balance(std::shared_ptr<Tradeset> iTradeset);

		// Non-const methods
		void update() override;
		void clear();

		// Const methods
		inline float value() const;

	private:

		float value_;
		Currency ccy_;
		// Observed
		std::shared_ptr<Tradeset> iTradeset_;
	};


	// Inline definitions

	inline float Balance::value() const { return value_; }

}
