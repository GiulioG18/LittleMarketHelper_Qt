
#include <sstream>

#include "Http/ConfigRequest.h"
#include "Config.h"
#include "Http/Curl.h"


namespace lmh {

	/*
	ConfigRequest::ConfigRequest(const std::string& type)
		:
		path_("httpRequest." + type)
	{
	}

	Status ConfigRequest::fetch(const std::string& filler)
	{
		// Run all available API requests until successful
		for (const pt::ptree::value_type& api : Config::getChild(path_))
		{
			Status status = Status::SUCCESS;

			stats_.clear();

			// TODO: add Stats
			stats_.name_ = api.first;
			stats_.method_ = api.second.get<std::string>("method");
			stats_.url_ = api.second.get<std::string>("url");

			// Run HTTP request
			if (stats_.method_ == "get")
			{
				replacePlaceholder(stats_.url_, filler);
				status = Curl::get().GETRequest(stats_.url_);
			}
			else if (stats_.method_ == "post")
			{
				std::string data = api.second.get<std::string>("data");
				replacePlaceholder(data, filler);
				status = Curl::get().POSTRequest(stats_.url_, data);
			}
			else
				FAIL("unknown method");

			if (status != Status::SUCCESS)
				break;

			// Check response format is valid JSON
			std::stringstream ss;
			ss << Curl::get().response();
			status = response_.parse(ss);
			if (status != Status::SUCCESS)	// TODO: create macro to do something on return on error
				break;



		}

		return Status::SUCCESS;
	}

	void ConfigRequest::clear()
	{
		path_.clear();
		response_.clear();
	}

	void ConfigRequest::ofType(const std::string& type)
	{
		clear();
		path_ = "httpRequest." + type;
	}

	Status ConfigRequest::get(const std::string& content)
	{
		return Status();
	}

	Status ConfigRequest::post(const std::string& content)
	{
		return Status();
	}

	void ConfigRequest::replacePlaceholder(std::string& s, const std::string& value)
	{
		auto pos = s.find("%%%PLACEHOLDER%%%");
		if (pos == std::string::npos)
			return;

		// Replace it (%%%PLACEHOLDER%%% is 17 letters)
		s.replace(pos, 17, value);
	}


	std::string ConfigRequest::GET(const std::string& type, const std::string& urlPlaceholder, Status* ec)
	{
		// Config must be initialized
		Config& config = Config::get();
		if (!config.initialized())
			*ec = Status::CONFIG_NOT_INITIALIZED;

		// Curl must be initialized
		Curl& curl = Curl::get();
		if (!curl.initialized())
			*ec = Status::CURL_NOT_INITIALIZED;

		if (*ec == Status::SUCCESS)
		{
			std::string path = "httpRequest.get." + type + ".url";

			// Try to read url in config file
			auto url = config.read<std::string>(path);
			if (!url.has_value())
			{
				*ec = Status::CONFIG_INVALID_PATH;
				return std::string();
			}
			replacePlaceholder(url.value(), urlPlaceholder);

			*ec = curl.GETRequest(url.value());
			if (*ec == Status::SUCCESS)
				return curl.response();
		}

		return std::string();
	}

	std::string ConfigRequest::POST(const std::string& what, const std::string& dataPlaceholder, Status* ec)
	{
		// Config must be initialized
		Config& config = Config::get();
		if (!config.initialized())
			*ec = Status::CONFIG_NOT_INITIALIZED;

		// Curl must be initialized
		Curl& curl = Curl::get();
		if (!curl.initialized())
			*ec = Status::CURL_NOT_INITIALIZED;

		if (*ec == Status::SUCCESS)
		{
			std::string basePath = "httpRequest.post." + what;

			// Try to read url in config file
			auto url = config.read<std::string>(basePath + ".url");
			if (!url.has_value())
			{
				*ec = Status::CONFIG_INVALID_PATH;
				return std::string();
			}

			// Try to read data in config file
			auto data = config.read<std::string>(basePath + ".data");
			if (!url.has_value())
			{
				*ec = Status::CONFIG_INVALID_PATH;
				return std::string();
			}
			replacePlaceholder(data.value(), dataPlaceholder);


			*ec = curl.POSTRequest(url.value(), data.value());
			if (*ec == Status::SUCCESS)
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
	*/
}