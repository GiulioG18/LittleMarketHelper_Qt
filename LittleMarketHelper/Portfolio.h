
// ========================================================================
//		Financial portfolio
// ========================================================================

#pragma once

#include "Null.h"
#include "PortfolioTrades.h"


namespace lmh {

	class Portfolio
	{
	public:

		Portfolio()
			: balance_(Null<float>()), initialized_(false), calibrated_(false)
		{
		}
		

	private:

		PortfolioTrades trades_;
		float balance_;
		bool initialized_;
		bool calibrated_;
	};
}
