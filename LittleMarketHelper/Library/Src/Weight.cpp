
#include "Weight.h"
#include "Security.h"
#include "Balance.h"


namespace lmh {

	Weight::Weight(Security* security, std::shared_ptr<Balance> balance)
		: value_(0.0), security_(security), balance_(std::move(balance))
	{
		REQUIRE(security_, "invalid security");
		REQUIRE(balance_, "invalid balance");

		registerWith(balance_);
	}

	void Weight::update() 
	{
		REQUIRE(security_, "invalid security");
		REQUIRE(balance_, "invalid balance");
		REQUIRE(security_->quote().price().amount() > 0.0, "invalid price");
		REQUIRE(security_->quantity() >= 0, "invalid quantity");

		if (balance_->price().amount() == 0.0)
		{
			ASSERT(security_->marketValue().amount() == 0.0, "balance is 0, but security market value is positive");
			value_ = 0.0;
		}
		else
		{
			value_ = (security_->marketValue() / balance_->price()).amount();
		}

		ENSURE(value_ >= 0 && value_ <= 1, "weight outside [0, 1] range");
	}	

}