
// ========================================================================
//		Weight class
// ========================================================================

#pragma once

#include <memory>

#include "Null.h"
#include "Assertions.h"
#include "FinProduct.h"
#include "Balance.h"


namespace lmh {

	template<typename Type>
	class Weight : public Observer
	{
	public:

		static_assert(std::is_arithmetic<Type>::value, "Only arithmetic types are allowed");
		static_assert(!std::is_same<Type, bool>::value, "Booleans not allowed");

	public:

		Weight(Type value);

		virtual void update() = 0;

		// Non-const methods

		// Const methods
		inline Type value() const;

	protected:

		Type value_;
	};

	
	// Actual 

	class ActualWeight : public Weight<float>
	{
		// Observes a Balance. 
		// NB: there is no need to track the paired
		// product as well, since tracking the balance is enough

	public:

		ActualWeight(std::shared_ptr<FinProduct> product, std::shared_ptr<Balance> balance);

		// Non-const methods
		virtual void update() override;

	private:

		std::shared_ptr<FinProduct> product_;	// Paired product
		// Observed
		std::shared_ptr<Balance> balance_;
	};


	// Target

	class TargetWeight : public Weight<float>
	{
		// Tracks a target object

	public:

		TargetWeight();

		// Non-const methods
		virtual void update() override;
	};


	// Estimated

	class EstimatedWeight : public Weight<float>
	{
		// Tracks an estimate object

	public:

		EstimatedWeight();

		// Non-const methods
		virtual void update() override;
	};


	// Inline definitions

	template<typename Type>
	inline Type Weight<Type>::value() const
	{
		return value_;
	}

	template<typename Type>
	inline Weight<Type>::Weight(Type value)
		: value_(value)
	{
	}

}

