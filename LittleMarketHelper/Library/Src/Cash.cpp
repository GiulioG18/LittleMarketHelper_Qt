
#include "Cash.h"


namespace lmh {

	Cash::Cash(Ccy ccy, double value)
		: price_(ccy, value)
	{
	}

	bool Cash::Comp::operator()(const Cash& first, const Cash& second) const
	{
		return first.price().ccy() < second.price().ccy();
	}

	bool Cash::Comp::operator()(const Ccy& first, const Cash& second) const
	{
		return first < second.price().ccy();
	}

	bool Cash::Comp::operator()(const Cash& first, const Ccy& second) const
	{
		return first.price().ccy() < second;
	}
}