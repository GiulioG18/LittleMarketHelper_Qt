
#include <boost/algorithm/string.hpp>

#include "Http/Api.h"
#include "Config.h"
#include "ExchangeRate.h"


namespace lmh {

	namespace http {


		// Base

		Status Api::send()
		{
			Curl& curl = Curl::get();

			// Reset stats
			lastStats_ = std::make_unique<Stats>();

			// Send request
			Status status;
			if (method_ == "get")
				status = sendGetRequest();
			else if (method_ == "post")
				status = sendPostRequest();
			else
				status = Status::API_INVALID_METHOD;

			if (status != Status::SUCCESS)
				return status;

			// Write stats
			// TODO: impl

			// Write response
			response_ = curl.response();
			return Status::SUCCESS;
		}
											
		Status Api::writeJson()
		{
			std::stringstream ss;
			ss << response_;
			return json_.parse(ss);
		}

		Status Api::sendGetRequest()
		{
			Status status = Status::SUCCESS;

			// Validate input
			REQUIRE(!url_.empty(), "empty url");

			replacePlaceholder(url_, filler_);
			return Curl::get().GETRequest(url_);
		}

		Status Api::sendPostRequest()
		{
			Status status = Status::SUCCESS;

			// Validate input
			REQUIRE(!url_.empty(), "empty url");
			REQUIRE(!filler_.empty(), "empty filler for POST request");
			REQUIRE(!data_.empty(), "empty data for POST request");

			replacePlaceholder(data_, filler_);
			return Curl::get().POSTRequest(url_, data_);
		}

		void Api::replacePlaceholder(std::string& s, const std::string& value) const
		{
			std::string placeholder = std::string("%%%PLACEHOLDER%%%");

			auto pos = s.find(placeholder);
			if (pos == std::string::npos)
				return;

			s.replace(pos, placeholder.length(), value);
		}


		// Api stats

		void Api::Stats::clear()
		{
			bytes_ = static_cast<size_t>(0);
			time_ = std::chrono::milliseconds::zero();
		}


		// Quote

		ConnectionTest::ConnectionTest()
		{
			std::string basePath = std::string("httpRequest.connectionTest.");
			
			method_ = Config::properties().get<std::string>(basePath + "method");
			url_ = Config::properties().get<std::string>(basePath + "url");
			data_ = ""; 
			filler_ = "";
			keys_.push_back(Config::properties().get<std::string>(basePath + "key"));
		}

		bool ConnectionTest::run()
		{
			// Run http request
			Status status = Api::send();
			if (status != Status::SUCCESS)
				return false;
			
			// Parse result into Json file
			status = Api::writeJson();
			if (status != Status::SUCCESS)
				return false;

			// Evaluate result
			auto msg = json_.tree().get_optional<std::string>(keys_.front());
			return msg.has_value(); // NB: a very cool message is lost here
		}

		Quote::Quote()
		{
			std::string path = "httpRequest.quoteByFullTicker.yahooFinance";
		}

		

		ExchangeRate::ExchangeRate()
		{
			std::string basePath = std::string("httpRequest.exchangeRate.");

			method_ = Config::properties().get<std::string>(basePath + "method");
			url_ = Config::properties().get<std::string>(basePath + "url");
			data_ = "";
			filler_ = "";
			keys_.push_back(Config::properties().get<std::string>(basePath + "key"));
		}

		std::set<lmh::ExchangeRate> ExchangeRate::run()
		{
			// TODO: should check if the date it is recent enough
			
			std::set<lmh::ExchangeRate> out;

			// Run http request
			Status status = Api::send();
			if (status != Status::SUCCESS)
				return out;

			// Parse result into Json file
			status = Api::writeJson();
			if (status != Status::SUCCESS)
				return out;

			// Request rates for most common currencies
			std::string key = keys_.front();
			Currency xxx = Currency::EUR; // TODO: should we not hard code the base Currency?
			for (const auto& pair : ccy::map())
			{			
				Currency yyy = pair.first;
				std::string currency = pair.second;
				boost::algorithm::to_lower(currency);
				auto rate = json_.tree().get_optional<double>(key + "." + currency);
				if (rate.has_value())
				{
					// Construct rate
					lmh::ExchangeRate er(xxx, yyy, lmh::Quote({ yyy, rate.value()}, 0.0)); // TODO: once timestamp is properly impl, impl 

					// Move it into output set
					out.insert(std::move(er));
				}
			}

			return out;
		}

}
}