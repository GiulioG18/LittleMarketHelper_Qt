
// ========================================================================
//		Wrapper around foundamental types
// ========================================================================

#pragma once

#include "Patterns/Observable.h"

	// TODO: add types
#define uint32_t_obs lmh::ObservableFT<uint32_t>


namespace lmh {

	// TODO: move impl

	template<typename Type>
	requires std::is_fundamental_v<Type>
	class ObservableFT : public Observable
	{
	public:
		
		ObservableFT(Type x) : x_(x) {}

		void set(Type x)
		{
			x_ = x;

			notifyObservers();
		}

		const Type& get()
		{
			return x_;
		}

	private:

		Type x_;
	};

}