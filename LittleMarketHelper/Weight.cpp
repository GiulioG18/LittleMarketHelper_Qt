
#include "Weight.h"


namespace lmh {

	ActualWeight::ActualWeight(std::shared_ptr<FinProduct> product, std::shared_ptr<Balance> balance)
		: Weight<float>(Null<float>()), product_(std::move(product)), balance_(std::move(balance))
	{
		REQUIRE(balance_, "invalid balance");

		registerWith(balance_);
	}

	void ActualWeight::update()
	{
		REQUIRE(product_, "invalid product");
		REQUIRE(!Null<float>::check(product_->price()), "null price");
		REQUIRE(!Null<int>::check(product_->quantity()), "null quantity");
		REQUIRE(!Null<float>::check(balance_->value()), "null balance");

		if (balance_->value() == 0.0f)
		{
			REQUIRE(product_->openPosition() == 0.0f, 
				"balance is 0.0, product open position is positive");
			value_ = 0.0f;
		}
		else
		{
			value_ = product_->openPosition() / balance_->value();
		}

		ENSURE(value_ >= 0 || value_ <= 1, "weight outside [0,1] range");
	}	



	TargetWeight::TargetWeight()
		: Weight<float>(Null<float>())
	{
	}

	void TargetWeight::update()
	{
		// TODO: implement
		ASSERT(true, "not implemented");
	}



	EstimatedWeight::EstimatedWeight()
		: Weight<float>(Null<float>())
	{
	}

	void EstimatedWeight::update()
	{
		// TODO: implement
		ASSERT(true, "not implemented");
	}

}