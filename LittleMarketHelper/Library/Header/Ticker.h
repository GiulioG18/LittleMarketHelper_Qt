
// ========================================================================
//		Ticker class
// ========================================================================

#pragma once

#include <string>


namespace lmh {

	// Yahoo finance ticker
	class YTicker
	{
	public:

		YTicker() = default;

	private:

		std::string symbol_;
		std::string exchangeCode_;
	};

}