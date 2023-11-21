
#include "Quote.h"


namespace lmh {

	Quote::Quote(const Price& price)
		:
		price_(price)
	{
	}

	Quote::Quote(const Price& price, const Date& date)
		:
		price_(price),
		date_(date)
	{
	}

}