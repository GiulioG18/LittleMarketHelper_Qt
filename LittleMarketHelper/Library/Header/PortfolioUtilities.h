#pragma once

#include <string>

#include "Currency.h"


// To ignore const and ref when comparing types
template<class T1, class T2>
bool is_almost_same = std::is_same<const T1&, const T2&>;

namespace lmh {

	struct EditTrade
	{
		enum Type
		{
			NAME,
			CURRENCY,
			QUANTITY,
			PRICE
		};

		static bool validateEdit(EditTrade::Type t, auto value)
		{
			switch (t)
			{
			case EditTrade::NAME:		return is_almost_same<decltype(value), std::string>;	break;
			case EditTrade::CURRENCY:	return is_almost_same<decltype(value), Currency>;		break;
			case EditTrade::QUANTITY:	return is_almost_same<decltype(value), int>;			break;
			case EditTrade::PRICE:		return is_almost_same<decltype(value), float>;			break;

			default:					return false;											break;
			}
		}
	};	

}
