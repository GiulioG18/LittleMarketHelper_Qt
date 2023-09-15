
#include "Utils/Warnings.h"

#ifdef _WIN32
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "crypt32.Lib")
#pragma comment(lib, "normaliz.lib")
#pragma comment(lib, "ws2_32.Lib")
#pragma comment(lib, "wldap32.Lib")
#else
static_assert(false, "I had no clue nor interest in linking these libs for other OS");
#endif

#include "Utils/Curl.h"
#include "Utils/Logger.h"


// Macro to handle Curl errors
#define CC(x) do {																				\
  CURLcode code = (x);																			\
  if (code != CURLE_OK) {																		\
    return code;																				\
  }																								\
} while (0)



namespace lmh {

	// Curl instance
	CURL* _curl;

	// Define a callback function for writing data
	size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output)
	{
		size_t totalSize = size * nmemb;
		output->append(static_cast<char*>(contents), totalSize);
		return totalSize;
	}


	// Curl

	Curl::~Curl()
	{
		if (initialized_)
		{
			curl_easy_cleanup(_curl);
			curl_global_cleanup();
		}
	}

	CURLcode Curl::initialize(long flag)
	{
		if (!initialized_)
		{
			// Init curl
			if (flag == -1)
			{
				CC(curl_global_init(CURL_GLOBAL_ALL));
			}
			else
			{
				CC(curl_global_init(flag));
			}

			// init version
			version_ = curl_version_info(CURLVERSION_NOW);				

			// Initialize instance
			_curl = curl_easy_init();

			initialized_ = true;
		}

		return CURLE_OK;
	}

	/*Curl::Handle Curl::createhandle()
	{
		REQUIRE(Curl::instance().initialized_, "Curl has not been initialized");

		return Handle();
	}*/

	std::string Curl::explainCode(CURLcode status) const
	{
		return std::string(curl_easy_strerror(status));
	}

	CURLcode Curl::GETRequest(const std::string& URL)
	{
		REQUIRE(Curl::instance().initialized_, "Curl has not been initialized");
		REQUIRE(_curl, "invalid curl handle");

		// Set URL 
		CC(curl_easy_setopt(_curl, CURLOPT_URL, URL.c_str()));

		// Set the callback function for writing received data
		CC(curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, writeCallback));
		CC(curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response_));

		// Perform the request
		CC(curl_easy_perform(_curl));
		return CURLE_OK;
	}

	


	// Handle

	

	//Curl::Handle::Handle() 
	//{
	//	// Initialize curl
	//	curl_ = std::unique_ptr<CURL, CurlDeleter>(curl_easy_init());

	//	// Redirect debugger into logfile
	//	initLog();

	//	// Initialize response
	//	curl_easy_setopt(curl_.get(), CURLOPT_WRITEDATA, &response_);
	//}

	//CURLcode Curl::Handle::status() const
	//{
	//	return CURLE_OK;
	//}

	//CURLcode Curl::Handle::reset()
	//{
	//	curl_easy_reset(curl_.get());
	//	ENSURE(curl_, "invalid curl handle");

	//	CC(initLog());

	//	return CURLE_OK;
	//}

	//CURLcode Curl::Handle::setOption(CURLoption option, auto parameter)
	//{
	//	REQUIRE(curl_, "invalid curl handle");
	//	CC(curl_easy_setopt(curl_, option, parameter));

	//	return CURLE_OK;
	//}

	//CURLcode Curl::Handle::GETRequest(const std::string& URL)
	//{
	//	REQUIRE(curl_, "invalid curl handle");

	//	// Set URL 
	//	CC(curl_easy_setopt(curl_.get(), CURLOPT_URL, URL.c_str()));

	//	// Set the callback function for writing received data
	//	CC(curl_easy_setopt(curl_.get(), CURLOPT_WRITEFUNCTION, WriteCallback));
	//	CC(curl_easy_setopt(curl_.get(), CURLOPT_WRITEDATA, &response_));


	//	// Perform the request
	//	CC(curl_easy_perform(curl_.get()));

	//	return CURLE_OK;
	//}

	//CURLcode Curl::Handle::POSTRequest()
	//{
	//	REQUIRE(curl_, "invalid curl handle");

	//	return CURLE_OK;
	//}

	//CURLcode Curl::Handle::initLog()
	//{
	//	REQUIRE(curl_, "invalid curl handle");
	//	Logger& logger = Logger::instance();
	//	if (logger.initialized())
	//	{
	//		if (logger.logLevel() >= LOG_LEVEL_DEBUG)
	//		{
	//			CC(curl_easy_setopt(curl_.get(), CURLOPT_STDERR, logger.stream()));
	//			CC(curl_easy_setopt(curl_.get(), CURLOPT_VERBOSE, 1L));
	//		}
	//	}

	//	return CURLE_OK;
	//}

}

