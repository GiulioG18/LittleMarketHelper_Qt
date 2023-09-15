
#include "Weight.h"


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