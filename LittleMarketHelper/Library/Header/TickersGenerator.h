
// ========================================================================
//		Tickers generator		
// ========================================================================

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Utils/StatusCode.h"
#include "Utils/Json.h"


namespace lmh{

	// This class generates a map of ticker and exchange codes
	// from a response of a FIGI request
	class Figi
	{
	public:

		//										ticker			 exchange codes
		using TickersMap = std::unordered_map<std::string, std::vector<std::string>>;

	public:

		Figi() = default;

		Status generateTickers();

		// Getters
		inline std::string response() const;
		inline TickersMap tickers() const;

	public:

		Status parse(std::string_view response);

	private:

		Json json_;
		std::string response_;
		TickersMap tickers_;
	};


	// Inline definitions
	inline std::string Figi::response() const { return response_; };
	inline Figi::TickersMap Figi::tickers() const { return tickers_; };

}
