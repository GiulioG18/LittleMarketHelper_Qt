
#include "Quote.h"


namespace lmh {

	Quote::Quote(const Price& price)
		:
		price_(price)
	{
	}

	Quote::Quote(const Price& price, const Timestamp& timestamp)
		:
		price_(price),
		timestamp_(timestamp)
	{
	}

}