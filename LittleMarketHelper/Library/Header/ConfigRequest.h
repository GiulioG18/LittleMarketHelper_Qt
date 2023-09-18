
//// ========================================================================
////		Helper for Config based HTTP requests 
//// ========================================================================

#pragma once

#include "Utils/StatusCode.h"

namespace lmh {

	// Helper class to launch HTTP requests based on config file
	// Both Curl and Config objects must be initialized

	class ConfigRequest
	{
	public:

		ConfigRequest() = default;
		// Returns the response (string) of the given request with details specified in the config file.
		// NB: this function is expecting to receive a JSON formatted response, but there is no checking done
		//	   on the response at the moment
		// NB2: if *ec is not LmhStatus::SUCCESS, it will return right away with the previous error
		static std::string GET(const std::string& type, const std::string& urlPlaceholder, LmhStatus* ec);
		static std::string POST(const std::string& type, const std::string& dataPlaceholder, LmhStatus* ec);
		// Replace the placeholder delimited by %%% with value
		// NB: this function expects a single placeholder (ie: "aBc2%%%PLACEHOLDER%%%888R")
		//     otherwise its behaviour is undefined
		static void replacePlaceholder(std::string& s, const std::string& value);
	};

}