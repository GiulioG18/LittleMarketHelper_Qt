
#include "ConfigRequest.h"
#include "Config.h"
#include "Utils/Curl.h"


namespace lmh {

	std::string ConfigRequest::GET(const std::string& type, const std::string& urlPlaceholder, LmhStatus* ec)
	{
		// Config must be initialized
		Config& config = Config::instance();
		if (!config.initialized())
			*ec = LmhStatus::CONFIG_NOT_INITIALIZED;

		// Curl must be initialized
		Curl& curl = Curl::instance();
		if (!curl.initialized())
			*ec = LmhStatus::CURL_NOT_INITIALIZED;

		if (*ec == LmhStatus::SUCCESS)
		{
			std::string path = "httpRequest.get." + type + ".url";

			// Try to read url in config file
			auto url = config.read<std::string>(path);
			if (!url.has_value())
			{
				*ec = LmhStatus::CONFIG_INVALID_PATH;
				return std::string();
			}
			replacePlaceholder(url.value(), urlPlaceholder);

			*ec = curl.GETRequest(url.value());
			if (*ec == LmhStatus::SUCCESS)
				return curl.response();
		}

		return std::string();
	}

	std::string ConfigRequest::POST(const std::string& type, const std::string& dataPlaceholder, LmhStatus* ec)
	{
		// Config must be initialized
		Config& config = Config::instance();
		if (!config.initialized())
			*ec = LmhStatus::CONFIG_NOT_INITIALIZED;

		// Curl must be initialized
		Curl& curl = Curl::instance();
		if (!curl.initialized())
			*ec = LmhStatus::CURL_NOT_INITIALIZED;

		if (*ec == LmhStatus::SUCCESS)
		{
			std::string basePath = "httpRequest.post." + type;

			// Try to read url in config file
			auto url = config.read<std::string>(basePath + ".url");
			if (!url.has_value())
			{
				*ec = LmhStatus::CONFIG_INVALID_PATH;
				return std::string();
			}

			// Try to read data in config file
			auto data = config.read<std::string>(basePath + ".data");
			if (!url.has_value())
			{
				*ec = LmhStatus::CONFIG_INVALID_PATH;
				return std::string();
			}
			replacePlaceholder(data.value(), dataPlaceholder);


			*ec = curl.POSTRequest(url.value(), data.value());
			if (*ec == LmhStatus::SUCCESS)
				return curl.response();
		}

		return std::string();
	}

	void ConfigRequest::replacePlaceholder(std::string& s, const std::string& value)
	{
		auto pos = s.find("%%%PLACEHOLDER%%%");
		if (pos == std::string::npos)
			return;

		// Replace it (%%%PLACEHOLDER%%% is 17 letters)
		s.replace(pos, 17, value);
	}

}