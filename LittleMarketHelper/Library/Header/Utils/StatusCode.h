
// ========================================================================
//		Status returned
// ========================================================================

#pragma once

#include <string>


#define CODE LmhStatusCode

enum class LmhStatusCode
{
	// All is good
	SUCCESS = 0,
	// The selected Currency is not allowed
	CURRENCY_NOT_ALLOWED,
	// The trade was not found
	TRADE_NOT_FOUND,
	// Invalid value passed for EDIT operation
	INVALID_EDIT,

};

namespace lmh {

	inline std::string CodeToString(LmhStatusCode code)
	{
		switch (code)
		{
		case LmhStatusCode::SUCCESS:
			break;
		case LmhStatusCode::CURRENCY_NOT_ALLOWED:
			break;
		case LmhStatusCode::TRADE_NOT_FOUND:
			break;
		case LmhStatusCode::INVALID_EDIT:
			break;
		default:
			break;
		}

		return std::string();
	}

}

