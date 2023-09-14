
// ========================================================================
//		Weight class
// ========================================================================

#pragma once

#include <memory>

#include "Utils/Assertions.h"
#include "Security.h"
#include "Balance.h"

void gogo();


namespace lmh {

	class Weight : public Observer
	{
	public:

		Weight(std::shared_ptr<Security> security, std::shared_ptr<Balance> balance);

		// Non-const methods
		virtual void update() override;

		// Const methods
		inline float value() const;

	private:

		float value_;
		std::shared_ptr<Security> security_;	// Paired security
		std::shared_ptr<Balance> balance_;		// Observed
	};


	// Inline definitions

	inline float Weight::value() const
	{
		ASSERT(value_ >= 0 || value_ <= 1, "weight outside [0, 1] range");
		return value_;
	}

}

