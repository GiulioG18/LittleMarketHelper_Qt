
// ========================================================================
//		Interface for object with lazy calculations
// ========================================================================

#pragma once

#include "Patterns/Observable.h"
#include "Utils/Assertions.h"


namespace lmh {

	class LazyObject :
		public virtual Observable,
		public virtual Observer
	{
	public:

		LazyObject();
		~LazyObject() override = default;

		void update() override;
		// Forces calculations by momentarily setting frozen_ and 
		// calculated_ to false before calling 'calculate()'
		// NB: if the object has been correctly registered with 
		// the needed observables, this should never be needed
		void recalculate();
		void freeze();
		void unfreeze();
		void notifyAlways();
		void notifyOnce();

	protected:

		virtual void calculate();
		virtual void performCalculations() = 0;

	private:

		bool updating_;
		bool calculated_;
		bool frozen_;
		// NB: a frozen object will not notify
		bool notifyAlways_;

		struct Updater
		{
			LazyObject* caller_;

			Updater(LazyObject* caller)
				: caller_(caller)
			{
				caller_->updating_ = true;
			}

			~Updater()
			{
				caller_->updating_ = false;
			}
		};
	};
					
	// Definitions

	LazyObject::LazyObject()
		: 
		calculated_(false),
		frozen_(false),
		updating_(false),
		notifyAlways_(false)
	{
	}

	inline void LazyObject::update()
	{
		ASSERT(!updating_, "object already updating, there is probably an object cycle");

		LazyObject::Updater updater(this);

		if (calculated_ || notifyAlways_)
		{
			calculated_ = false;

			if (!frozen_)
			{
				notifyObservers();
			}
		}
	}

	inline void LazyObject::recalculate()
	{
		bool frozen = frozen_;
		calculated_ = frozen = false;

		calculate();

		frozen_ = frozen;
		notifyObservers();
	}

	inline void LazyObject::freeze()
	{
		frozen_ = true;
	}

	inline void LazyObject::unfreeze()
	{
		if (frozen_)
		{
			frozen_ = false;
			notifyObservers();
		}
	}

	inline void LazyObject::calculate()
	{
		if (!calculated_ && !frozen_)
		{
			calculated_ = true;
			performCalculations();
		}
	}

	inline void LazyObject::notifyAlways()
	{
		notifyAlways_ = true;
	}

	inline void LazyObject::notifyOnce()
	{
		notifyAlways_ = false;
	}
}