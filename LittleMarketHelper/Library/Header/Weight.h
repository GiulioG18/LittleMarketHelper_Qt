
// ========================================================================
//		Weight class
// ========================================================================

#pragma once

#include <memory>

#include "Utils/Null.h"
#include "Utils/Assertions.h"
#include "FinProduct.h"
#include "Balance.h"


namespace lmh {

	class Weight : public Observer
	{
	public:

		Weight(std::shared_ptr<FinProduct> product, std::shared_ptr<Balance> balance);

		// Non-const methods
		virtual void update() override;

		// Const methods
		inline float value() const;

	private:

		float value_;
		std::shared_ptr<FinProduct> product_;	// Paired product
		// Observed
		std::shared_ptr<Balance> balance_;
	};


	// Inline definitions

	inline float Weight::value() const
	{
		ASSERT(value_ >= 0 || value_ <= 1, "weight outside [0, 1] range");
		return value_;
	}

}

