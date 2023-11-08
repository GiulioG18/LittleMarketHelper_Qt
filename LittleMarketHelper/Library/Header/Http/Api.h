
// ========================================================================
//		Http APIs
// ========================================================================

#pragma once

#include <chrono>
#include <memory>
#include <list>
#include <set>
#include <string_view>

#include "Http/Curl.h"
#include "Utils/Json.h"
#include "Utils/StatusCode.h"


namespace lmh {

	// Forward declarations
	class ExchangeRate;
	enum class Currency;

	// Fallback mechanism is not implemented.
	// Each class is a wrapper around an API defined in the configuration file.
	// To achieve more flexibility, url and data and other options can be defined in the configuration file,
	// but this does not solve the issue: what do we do if the API is not working anymore?
	
	namespace http {

		// Base 
		class Api
		{
		public:

			struct Stats;

		public:

			virtual ~Api() = default;

			// Perform request and writes response 
			// NB: response could be empty or invalid
			Status send();

		protected:

			Api() = default;

			// Write string response into Json
			virtual Status writeJson();

		private:

			Status sendGetRequest();
			Status sendPostRequest();
			void replacePlaceholder(std::string& s, std::string_view value) const;

		protected:

			std::string name_;
			std::string method_;
			std::string url_;
			std::string data_;
			std::string filler_;
			std::list<std::string> keys_;
			std::string response_;
			Json json_;
			std::unique_ptr<Stats> lastStats_;
		};

		// Api stats
		struct Api::Stats
		{
			Stats() { clear(); }

			void clear();

			size_t bytes_;	// Downloaded bytes
			std::chrono::duration<double> time_;
		};


		// Connection test
		class ConnectionTest : public Api
		{
		public:

			ConnectionTest();
			virtual ~ConnectionTest() = default;

			// Returns true if there is a valid network connection
			bool run();			
		};


		// Get lmh::Quote
		class Quote : public Api
		{
		public:

			Quote();
			virtual ~Quote() = default;

			// TODO: do not fecth all the historical price when using yf

		};


		// Get lmh::ExchangeRate
		class ExchangeRate : public Api
		{
		public:

			ExchangeRate();
			virtual ~ExchangeRate() = default;

			// Returns a list of rates
			std::set<lmh::ExchangeRate> run(Currency baseCcy);
		};


		// Get a security's ticker (std::string) from an Isin (std::string)
		class Ticker : public Api
		{
		public:

			Ticker();
			virtual ~Ticker() = default;
		};

	}

	/*
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
			//*status = Curl::get().POSTRequest(figiUrl, figiData);
			//if (*status != Status::SUCCESS)
			//	return std::nullopt;

			//// Check response format is valid JSON (remove outer square brackets first)
			//Json response;
			//std::stringstream ss;
			//ss << Curl::get().response();
			//*status = response.parse(ss);
			//if (*status != Status::SUCCESS)
			//	return std::nullopt;
			////std::cout << Curl::get().response() << std::endl;

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

			return getQuoteFromTicker(ticker.amount());
		}
	};
	*/
}