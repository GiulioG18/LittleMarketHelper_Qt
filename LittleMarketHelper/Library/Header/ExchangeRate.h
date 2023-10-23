// ========================================================================
//		Exchange Rate class
// ========================================================================

#pragma once

#include "Currency.h"
#include "Utils/Nullable.h"
#include "Patterns/Observable.h"

namespace lmh {

	class ExchangeRate : public Observable
	{

	private:

		double_n rate_;
		Currency x_;
		Currency y_;
	};

}
