#pragma once

#include <string>

#include "Utils/TypeTraits.h"


namespace lmh {

	struct Edit
	{
		enum Type
		{
			NAME,
			QUANTITY,
			PRICE
		};

		template<Edit::Type t>
		inline static bool validateEditType(auto value)
		{
			if		constexpr (t == Edit::NAME)
				return is_almost_same_v<decltype(value), std::string>;
			else if constexpr (t == Edit::QUANTITY)
				return is_almost_same_v<decltype(value), uint32_t>;
			else if constexpr (t == Edit::PRICE)
				return is_almost_same_v<decltype(value), double>;
			else
				return false;
		}
	};	

}
