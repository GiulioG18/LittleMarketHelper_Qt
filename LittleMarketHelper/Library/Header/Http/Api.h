
// ========================================================================
//		Http APIs
// ========================================================================

#pragma once

#include <chrono>
#include <memory>
#include <set>

#include "Http/Curl.h"
#include "Utils/Json.h"
#include "Utils/StatusCode.h"


namespace lmh {

	// TODO: implement fallback mechanism
	
	// TODO: Create url and data class 
	// they need to inherit from some sort of "fillable" class 
	// the classes perform a check on the validity of the object

	namespace http {

		// Base 

		class Api
		{
		public:

			struct Stats;

		public:

			virtual ~Api() = default;

			// Perform request and writes results 
			// NB: response could be empty or invalid
			Status send();

		protected:

			Api() = default;

			// Write string response into Json
			virtual Status writeJson();

		private:

			Status sendGetRequest();
			Status sendPostRequest();
			void replacePlaceholder(std::string& s, const std::string& value) const;

		protected:

			std::string name_;
			std::string method_;
			std::string url_;
			std::string data_;
			std::string filler_;
			std::set<std::string> keys_;
			std::string response_;
			Json json_;
			std::unique_ptr<Stats> lastStats_;
		};


		// Api stats
		struct Api::Stats
		{
			Stats()
			{
				clear();
			}

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
			
		};


		// Get lmh::Quote
		class Quote : public Api
		{
		public:

			Quote();
			virtual ~Quote() = default;

		};


		// Get lmh::ExchangeRate
		class ExchangeRate : public Api
		{
		public:

			ExchangeRate();
			virtual ~ExchangeRate() = default;
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

			return getQuoteFromTicker(ticker.value());
		}
	};
	*/
}