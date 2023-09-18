
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
	class FigiAPI
	{
	public:

		//										ticker			 exchange codes
		using TickersMap = std::unordered_map<std::string, std::vector<std::string>>;

	public:

		FigiAPI() = default;

		LmhStatus generateTickers();

		// Getters
		inline std::string response() const;
		inline TickersMap tickers() const;

	public:

		LmhStatus parse(const std::string& response);

	private:

		Json json_;
		std::string response_;
		TickersMap tickers_;
	};


	// Inline definitions
	inline std::string FigiAPI::response() const { return response_; };
	inline FigiAPI::TickersMap FigiAPI::tickers() const { return tickers_; };

}
