
// ========================================================================
//		Security ISIN code
// ========================================================================

#pragma once

#include <string>


namespace lmh {

	class Isin
	{
	public:

		Isin(std::string_view isin); // [ MAY THROW ]

		bool operator<(const Isin& other) const;

	private:

		std::string value_;
	};

}