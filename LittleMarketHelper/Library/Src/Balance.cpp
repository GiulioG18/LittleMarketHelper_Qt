
#include "Balance.h"
#include "Utils/Assertions.h"
#include "Tradeset.h"


namespace lmh {

	Balance::Balance(Currency ccy, std::shared_ptr<Tradeset> trades)
		: value_(0.0f), ccy_(ccy), trades_(std::move(trades))
	{
		REQUIRE(trades_->get().empty(), 
			"Balance object must linked to an initially empty Tradeset");
		REQUIRE(trades_, "invalid trades");
		registerWith(trades_);			
	}

	void Balance::update()
	{
		value_ = 0.0f;
		for (const auto& trade : trades_->get())
		{
			// For now only single currency is supported 
			ASSERT(trade.first->ccy() == this->ccy_, "only single ccy is supported");
			value_ += trade.first->openPosition();
		}

		notifyObservers();		
	}

	void Balance::clear()
	{
		value_ = 0.0f;
		trades_->clear();
		notifyObservers();
	}

}