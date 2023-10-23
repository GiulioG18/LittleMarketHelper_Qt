
//// ========================================================================
////		Helper for Config based HTTP requests 
//// ========================================================================

#pragma once

#include <string>

#include "Utils/StatusCode.h"
#include "Utils/Json.h"
#include <unordered_map>


namespace lmh {

	// Helper class to launch HTTP requests based on config file
	// Both Curl and Config objects must be initialized

	class ConfigRequest
	{
	public:

		ConfigRequest(const std::string& type);

		// Initializes the response string for the object requested
		Status fetch(const std::string& filler);
		void clear();
		// Calls clear internally
		void ofType(const std::string& type);
		// Replace the Placeholder (syntax must be %%%PLACEHOLDER%%%) in the string with content
		// NB: this function expects a single placeholder
		static void replacePlaceholder(std::string& s, const std::string& value);

	private:

		// Return the response (string) of the given request
		// NB: these methods are expecting to receive a JSON formatted response
		Status get(const std::string& content);
		Status post(const std::string& content);

	private:

		struct Stats
		{
			inline void clear()
			{
				name_ = method_ = url_ = "";
			}

			std::string name_;
			std::string method_;
			std::string url_;
			// TODO: add request time
		};

		std::string path_;
		Json response_;
		Stats stats_;
	};

}