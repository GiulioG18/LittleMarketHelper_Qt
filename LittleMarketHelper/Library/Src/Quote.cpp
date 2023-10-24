
#include "Quote.h"


namespace lmh {

	Quote::Quote(const Price& price, double timestamp)
		:
		price_(price),
		timestamp_(timestamp)
	{
	}

}