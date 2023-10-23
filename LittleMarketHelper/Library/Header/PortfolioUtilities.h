#pragma once

#include <string>


// To ignore const and ref when comparing types
template<class T1, class T2>
bool is_almost_same_v = std::is_same_v<const T1&, const T2&>;

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

		template<EditTrade::Type t>
		static bool validateEditType(auto value)
		{
			if		constexpr (t == EditTrade::NAME)
				return is_almost_same_v<decltype(value), std::string>;
			else if constexpr (t == EditTrade::QUANTITY)
				return is_almost_sam_v<decltype(value), int>;
			else if constexpr (t == EditTrade::PRICE)
				return is_almost_same_v<decltype(value), double>;
			else
				return false;
		}
	};	

}
