
#include "Balance.h"
#include "Assertions.h"
#include "Null.h"
#include "Tradeset.h"


namespace lmh {

	Balance::Balance(std::shared_ptr<Tradeset> iTradeset)
		: iTradeset_(std::move(iTradeset)), value_(Null<float>()), ccy_(Currency::Iso::EUR)
	{
		REQUIRE(iTradeset_, "invalid iTrades"); // TODO: THIS SHOULD FAIL!!!
		registerWith(iTradeset_);
	}

	void Balance::update()
	{
		value_ = 0.0f;
		for (const auto& trade : iTradeset_->trades())
		{		
			REQUIRE(!Null<int>::check(trade.first->quantity()), "null quantity");
			REQUIRE(!Null<float>::check(trade.first->price()), "null price");

			value_ += trade.first->openPosition();
		}

		notifyObservers();		
	}

}