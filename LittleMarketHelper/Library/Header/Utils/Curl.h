
// ========================================================================
//		Curl operations class
// ========================================================================


#include <string>
#include <functional>
#include <memory>

#include "Patterns/Singleton.h"
#include "Utils/StatusCode.h"
#include "Utils/Assertions.h"
#include "curl/curl.h"


namespace lmh {

	// Wrapper around libcurl
	// For now only HTTP requests are supported

	class Curl : public Singleton<Curl>
	{
	public:

		friend class Singleton<Curl>;
		class Handle;

	private:

		Curl() = default;

	public:

		// Following libcurl doc suggestions, in this way multiple calls 
		// to curl_global_init()/curl_global_cleanup() are avoided
		virtual ~Curl();
		// By default sets the flag to CURL_GLOBAL_ALL 
		CURLcode initialize(long flag = -1);
		// Creates and return a new EASY handle (this is not thread safe)
		// It is encouraged to reuse easy handles for repeated transfers
		// NB: Curl must be initialized before creating Handle objects
		[[nodiscard("Handle will be lost")]] static Handle createhandle();
		// Returns string format for CURLcode
		std::string explainCode(CURLcode status) const;
		// GET request
		CURLcode GETRequest(const std::string& URL);
		// It closes down and frees all resources previously associated with this easy handle
		//CURLcode curl_easy_cleanup(_curl);

	private:

		bool initialized_ = false;
		curl_version_info_data* version_ = nullptr;
		std::string response_;
	};



	// Custom deleter function for std::unique_ptr
	struct CurlDeleter 
	{
		void operator()(CURL* curl) const 
		{
			if (curl)
				curl_easy_cleanup(curl);
		}
	};

	//class Curl::Handle
	//{
	//public:

	//	// If curl has not been correctly initialized,  
	//	// curl is set to NULL
	//	Handle();
	//	// Gets all the options that were set in the source handle 
	//	Handle(const Handle & h) = delete;
	//	Handle(Handle && h) = delete;
	//	Handle& operator=(Handle&&) = default;
	//	Handle& operator=(const Handle&) = delete;
	//	//~Handle();

	//	// Returns the error code from last Handle function called
	//	CURLcode status() const;
	//	// Puts back the handle to the same state as it was in when it was created
	//	CURLcode reset();
	//	// Set option
	//	CURLcode setOption(CURLoption option, auto parameter);
	//	// GET request
	//	CURLcode GETRequest(const std::string& URL);
	//	// POST request
	//	CURLcode POSTRequest();

	//private:

	//	// Redirect stderr to the log file
	//	CURLcode initLog();

	//private:

	//	std::unique_ptr<CURL, CurlDeleter> curl_;
	//};

}

