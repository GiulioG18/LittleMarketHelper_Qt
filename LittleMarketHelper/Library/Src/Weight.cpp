
#include "Weight.h"

#ifdef _WIN32
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "crypt32.Lib")
#pragma comment(lib, "normaliz.lib")
#pragma comment(lib, "ws2_32.Lib")
#pragma comment(lib, "wldap32.Lib")
#endif
#include "curl/curl.h"
#include <iostream>
void gogo()
{
	CURL* curl;
	CURLcode res;

	// Initialize libcurl
	curl = curl_easy_init();
	if (curl)
	{
		// Set the API endpoint URL
		const char* url = "https://query1.finance.yahoo.com/v8/finance/chart/CSSPX.SW";
		std::string response_data;

		// Configure the libcurl options
		curl_easy_setopt(curl, CURLOPT_URL, url);

		// Perform the POST request
		res = curl_easy_perform(curl);

		// Check for errors
		if (res != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		}
		else {
			std::cout << curl_easy_strerror(res) << std::endl;
		}
		// Clean up and free resources
		curl_easy_cleanup(curl);
	}
}





namespace lmh {

	Weight::Weight(std::shared_ptr<Security> security, std::shared_ptr<Balance> balance)
		: value_(0.0f), security_(std::move(security)), balance_(std::move(balance))
	{
		REQUIRE(balance_, "invalid balance");

		registerWith(balance_);
	}

	void Weight::update()
	{
		REQUIRE(security_, "invalid product");
		REQUIRE(balance_, "invalid balance");
		REQUIRE(Security::validatePrice(security_->price()), "invalid price");
		REQUIRE(Security::validateQuantity(security_->quantity()), "invalid quantity");

		if (balance_->value() == 0.0f)
		{
			ASSERT(security_->openPosition() == 0.0f,
				"balance is 0, but security open position is positive");
			value_ = 0.0f;
		}
		else
		{
			value_ = security_->openPosition() / balance_->value();
		}

		ENSURE(value_ >= 0 && value_ <= 1, "weight outside [0, 1] range");
	}	

}