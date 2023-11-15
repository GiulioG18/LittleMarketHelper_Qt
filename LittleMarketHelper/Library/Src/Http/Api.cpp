
#include <boost/algorithm/string.hpp>

#include "Http/Api.h"
#include "Http/Curl.h"
#include "Config.h"
#include "ExchangeRate.h"
#include "Currency.h"


namespace lmh {

	namespace http {

		Status Api::testNetworkConnection()
		{
			assert(Curl::get().initialized());

			// Initialize request
			std::string path = basePath() + "networkConnection.";
			Curl::Method method = Curl::toMethod(Config::properties().get<std::string>(path + "method"));
			std::string url = Config::properties().get<std::string>(path + "url");
			std::string key = Config::properties().get<std::string>(path + "key");

			// Force the HTTP request every time
			static Curl::Request request{ method, url, "", true };
			auto response = Curl::httpRequest(request);
			if (response.code() != Status::SUCCESS)
				return Status::NO_NETWORK_CONNECTION;
			
			// Process response
			Json json;
			std::stringstream ss;
			ss << response.data();
			Status status = json.parse(ss);
			if (status != Status::SUCCESS)
				return Status::NO_NETWORK_CONNECTION;

			// Evaluate message
			auto msg = json.tree().get_optional<std::string>(key);
			if (msg.has_value()) // NB: a very cool message is lost here
				return Status::SUCCESS;
			else
				return Status::NO_NETWORK_CONNECTION;
		}

		std::set<ExchangeRate> Api::getExchangeRatesForThisCurrency(Currency currency)
		{
			assert(Curl::get().initialized());

			std::set<lmh::ExchangeRate> out;

			// Initialize request
			std::string path = basePath() + "exchangeRates.";
			Curl::Method method = Curl::toMethod(Config::properties().get<std::string>(path + "method"));
			std::string url = Config::properties().get<std::string>(path + "url");
			std::string key = Config::properties().get<std::string>(path + "key");

			// Run HTTP request
			static Curl::Request request{ method , url, "" };
			auto response = Curl::httpRequest(request);
			if (response.code() != Status::SUCCESS)
				return out;

			// Process response
			Json json;
			std::stringstream ss;
			ss << response.data();
			Status status = json.parse(ss);
			if (status != Status::SUCCESS)
				return out;

			// Request rates for most common currencies
			Currency xxx = currency;
			for (const auto& pair : ccy::map())
			{			
				Currency yyy = pair.first;
				std::string currency = pair.second;
				boost::algorithm::to_lower(currency);
				auto rate = json.tree().get_optional<double>(key + "." + currency);
				if (rate.has_value())
				{
					// Construct rate
					lmh::ExchangeRate er(xxx, yyy, rate.value());

					// Move it into output set
					out.insert(std::move(er));
				}
			}

			// TODO2: should check if the date it is recent enough
			return out;
		}

		constexpr std::string Api::basePath()
		{
			return std::string("httpRequest.");
		}

		void Api::replacePlaceholder(std::string& s, std::string_view value)
		{
			std::string placeholder = std::string("%%%PLACEHOLDER%%%");

			auto pos = s.find(placeholder);
			if (pos == std::string::npos)
				return;

			s.replace(pos, placeholder.length(), value);
		}
	}

}
