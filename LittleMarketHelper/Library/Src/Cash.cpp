//
//#include "Cash.h"
//
//
//namespace lmh {
//
//	Cash::Cash(Currency currency, double value)
//		: price_(currency, value)
//	{
//	}
//
//	bool Cash::Comp::operator()(const Cash& first, const Cash& second) const
//	{
//		return first.price().currency() < second.price().currency();
//	}
//
//	bool Cash::Comp::operator()(const Currency& first, const Cash& second) const
//	{
//		return first < second.price().currency();
//	}
//
//	bool Cash::Comp::operator()(const Cash& first, const Currency& second) const
//	{
//		return first.price().currency() < second;
//	}
//}

// TODO: DELETE