
// ========================================================================
//		Http APIs
// ========================================================================

#pragma once

#include <optional>
#include <chrono>
#include <vector>

#include "Quote.h"
#include "Config.h"
#include "Utils/Cache.h"


namespace lmh {

	// TODO: move implementation to cpp file

	// TODO: implement fallback mechanism
	
	// TODO: Create url and data class 
	// they need to inherit from some sort of "fillable" class 
	// the classes perform a check on the validity of the object

	class HttpApi
	{
	public:

		struct Stats
		{
			std::chrono::duration<double> time_{ 0.0 };
			size_t bytes_ = 0;	// Download
		};

	public:

		HttpApi() = default;

	private:		

		std::string method_;
		std::string url_;
		std::string data_;
		std::string filler_;
		std::vector<std::string> keys_;
		Stats lastTransfer_;
	};




	class OpenFigiApi : public HttpApi
	{
	public:

		std::optional<std::string> getTicker(const std::string& isin) const
		{
			//auto a = figiRequests_->has(isin); // TODO: avoid double lookup
			return std::string();

		}
		bool removeFirstTicker()
		{
			return false;
		}

	private:

		//static std::unique_ptr<Cache<std::string, Json>> figiRequests_;
	};




	class YahooFinanceApi : public HttpApi
	{
	public:

		// NB: this function removes tickers from the Json response while trying to get a Quote
		std::optional<Quote> getQuoteFromTicker(const std::string& ticker)
		{
			//// HTTP request
			//std::string figiUrl = Config::read<std::string>("httpRequest.tickerByIsin.openFigi.url");
			//std::string figiData = Config::read<std::string>("httpRequest.tickerByIsin.openFigi.data");
			//ConfigRequest::replacePlaceholder(figiData, isin);
			//*status = Curl::instance().POSTRequest(figiUrl, figiData);
			//if (*status != Status::SUCCESS)
			//	return std::nullopt;

			//// Check response format is valid JSON (remove outer square brackets first)
			//Json response;
			//std::stringstream ss;
			//ss << Curl::instance().response();
			//*status = response.parse(ss);
			//if (*status != Status::SUCCESS)
			//	return std::nullopt;
			////std::cout << Curl::instance().response() << std::endl;

			//// Scan JSON for either data or error node
			//pt::ptree::value_type actual = response.tree().get_
			//	auto it1 = response.tree().get_child().find("error");
			//if (it1 != response.tree().data()
			//	return std::nullopt;

			//	auto it2 = response.tree().find("data");
			//	if (it2 != response.tree().not_found())
			//		return std::nullopt;

			//	// 2. Run yf http request for this ticker.exchange pair

			//	// ...
			return std::nullopt;
		}
		// Calls getQuoteFromTicker internally
		std::optional<Quote> getQuoteFromIsin(const std::string& isin)
		{
			// 1. Run OpenFigi to get a possible ticker for this isin
			OpenFigiApi of;
			auto ticker = of.getTicker(isin);

			return getQuoteFromTicker(ticker.value());
		}
	};

}