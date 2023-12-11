
// ========================================================================
//		Securities dataset
// ========================================================================

#pragma once

#include <map>


namespace lmh {

	// Forward declarations
	class Security;
	class Isin;


	class SecuritiesDataset
	{
	public:

		SecuritiesDataset() = default;

	private:

		std::map<Isin, const Security> securities_;
	};

}

