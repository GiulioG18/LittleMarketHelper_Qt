
// ========================================================================
//		Curl operations wrapper
// ========================================================================

#pragma once

#include <memory>

#include "curl/curl.h"

#include "Utils/Warnings.h"
#include "Patterns/Singleton.h"
#include "Utils/StatusCode.h"
#include "Utils/Assertions.h"
#include "Utils/Cache.h"
#include "Utils/Chrono.h"


namespace lmh {

	// Forward declarations
	class Json;


	namespace http {

		// Wrapper around libcurl

		class Curl : public Singleton<Curl>
		{
		public:

			friend class Singleton<Curl>;

			class Request;
			class Response;

			// Supported methods
			enum class Method 
			{
				INVALID = -1,
				GET,
				POST
			};

			using ResponseCache = std::unique_ptr<Cache<Request, Response>>;

		private:

			Curl() = default;

		public:

			virtual ~Curl();
			// By default sets the flag to CURL_GLOBAL_ALL 
			// NB: does not check for a valid network connection
			static Status initialize(int64_t flag = -1);
			// Read or caches the response for the given HTTP request
			static Response httpRequest(const Request& request);
			// Convert string to method
			static Method stom(std::string_view str);
			// Convert method to string
			static std::string mtos(Method method);

			// Getters
			inline bool initialized() const;

		private:

			void processInternally(Response& response);

		private:

			ResponseCache responses_;
			bool initialized_ = false;
		};




		// Request

		class Curl::Request
		{
			friend class Curl;
			friend class Curl::Response;

		public:

			Request(Method method, std::string_view url, std::string_view data, bool skipCache = false); // [ MAY THROW ]

			constexpr auto key() const;

			// Operators
			auto operator<=>(const Request& other) const;
			bool operator==(const Request& other) const;

		private:

			Method method_;
			std::string	url_;
			std::string	data_;
			bool skipCache_;
		};




		// Response

		class Curl::Response
		{
		public:

			friend class Curl;

			using Speed = curl_off_t;

		private:

			Response(const Request& request);

			// Extract info from CURL handle
			void extractInfo(CURL*& curl);

		public:

			// Tries to create a Json from data
			std::optional<Json> toJson() const;
			void prettyPrint(std::ostream& stream) const;

			// Getters
			inline const Request& request() const;
			inline Status code() const;
			inline const std::string& data() const;
			inline const Date& date() const;
			inline const Milliseconds& duration() const;
			inline size_t bytes() const;
			inline Speed speed() const;
			inline int64_t localPort() const;

		private:

			Request	request_;
			uint64_t id_;
			Status code_;
			std::string data_;				
			Date date_;
			Milliseconds duration_;
			size_t bytes_;	
			Speed speed_; // Bytes/second
			int64_t localPort_;
		};



		// Curl Handle wrapper
		class HandleMaker
		{
		public:

			HandleMaker();
			~HandleMaker();

			inline CURL* const get() const;

		private:

			CURL* handle_;
		};



		// Inline definitions
		inline bool Curl::initialized() const { return initialized_; }; 
		inline const Curl::Request& Curl::Response::request() const { return request_; }
		inline Status Curl::Response::code() const { return code_; }
		inline const std::string& Curl::Response::data() const { return data_; }
		inline const Date& Curl::Response::date() const { return date_; }
		inline const Milliseconds& Curl::Response::duration() const { return duration_; }
		inline size_t Curl::Response::bytes() const { return bytes_; }
		inline Curl::Response::Speed Curl::Response::speed() const { return speed_; }
		inline int64_t Curl::Response::localPort() const { return localPort_; }

	}
		
}

