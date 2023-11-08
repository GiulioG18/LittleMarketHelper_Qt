
#include "Quote.h"


namespace lmh {

	// TODO2: should check if the date is recent enough

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