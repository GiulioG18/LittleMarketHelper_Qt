
// ========================================================================
//		Portfolio trades
// ========================================================================

#pragma once

#include <vector>

#include "FinProduct.h"
#include "Weights.h"
#include "ProductExcluder.h"
#include "Assertions.h"


namespace lmh {

	class PortfolioTrades
	{
	public:

		PortfolioTrades()
			: size_(0)
		{
		}

		// Getters

		inline size_t size() const;

	private:

		inline void checkVectorSizes() const;

	private:

		// Every method that operates on these members has to
		// to ensure that each vector is of the same size (size_)
		// and that the order is maintained 

		size_t size_;
		std::vector<FinProduct> products_;
		std::vector<Weights> weights_;
		std::vector<ProductExcluder> excludeFromCalculations_;
	};

}




// Inline definitions

inline size_t lmh::PortfolioTrades::size() const
{
	return size_;
}

inline void lmh::PortfolioTrades::checkVectorSizes() const
{
	ASSERT(
		size_ ==
		products_.size() ==
		weights_.size() ==
		excludeFromCalculations_.size(),
		("uncorrect vector sizes")
	);
}