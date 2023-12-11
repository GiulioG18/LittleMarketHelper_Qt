
// ========================================================================
//		Weight class
// ========================================================================

#pragma once

#include <memory>

#include "Utils/Assertions.h"
#include "Patterns/Observable.h"
#include "Balance.h"


namespace lmh {

	// Forward declarations
	class Security;


	// Tracks a Balance object and updates the weight for the given security
	// NB: it does not update the weight when the security notifies
	class Weight : public Observer
	{
	public:

		Weight(Security* security, std::shared_ptr<Observable> balance);

		// Non-const methods
		virtual void update() override;

		// Getters
		inline double value() const;

	private:

		double value_;
		Security* security_;
		std::shared_ptr<Observable> balance_; // Observed (balance)
	};


	// Inline definitions

	inline double Weight::value() const
	{
		ASSERT(value_ >= 0 || value_ <= 1, "weight outside [0, 1] range");
		return value_;
	}

}

