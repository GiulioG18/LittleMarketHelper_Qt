
// ========================================================================
//		Abstract class for trades containers
// ========================================================================

#pragma once

#include <set>

#include "Patterns/Observable.h"
#include "FinProduct.h"


namespace lmh {

	// Forward declarations
	template<typename Type>
	class Weight;


	class Tradeset :
		public Observable,
		public Observer
	{
	public:

		using Trade = std::pair<std::shared_ptr<FinProduct>, std::unique_ptr<Weight<float>>>;

	public:

		// For now it's allowed only to construct an empty trade set 
		// and add trades one by one with 'add()'
		Tradeset() = default;

		// Non-const methods
		void update() override;
		void add(Trade&& trade);
		void remove();
		void clear();

		// Const methods
		size_t size() const;
		inline const std::set<Trade>& trades() const;
		
	protected:

		std::set<Trade> trades_;
	};

	// Set uses comparison operator to check for equality
	// Product name is used to check for identical products
	bool operator<(const Tradeset::Trade& left, const Tradeset::Trade& right);


	// Inline definitions

	inline const std::set<Tradeset::Trade>& Tradeset::trades() const { return trades_; }

}
