
// ========================================================================
//		Abstract class for trades containers
// ========================================================================

#pragma once

#include <set>
#include <optional>

#include "Patterns/Observable.h"
#include "Security.h"


namespace lmh {

	// Forward declarations
	class Weight;


	class Tradeset :
		public Observable,
		public Observer
	{
	public:

		using Trade = std::pair<std::shared_ptr<Security>, std::unique_ptr<Weight>>;
		using Iterator = std::set<Trade>::iterator;

	public:

		// For now it's allowed only to construct an empty trade set 
		// and add trades one by one with 'add()'
		Tradeset() = default;
		Tradeset(const Tradeset&) = delete;
		Tradeset(Tradeset&&) = delete;
		Tradeset& operator=(const Tradeset&) = delete;
		Tradeset& operator=(Tradeset&&) = delete;

		// Non-const methods
		void update() override;
		bool insert(Trade&& trade);
		bool erase(const std::string& isin);
		std::optional<Trade> extract(const std::string& isin);
		void clear();

		// Const methods
		size_t size() const;
		Iterator find(const std::string& isin) const;
		inline const std::set<Trade>& get() const;
		
	private:

		std::set<Trade> trades_;
	};

	// std::set uses comparison operator to check for equivalence:
	// security Isin is used
	bool operator<(const Tradeset::Trade& left, const Tradeset::Trade& right);


	// Inline definitions

	inline const std::set<Tradeset::Trade>& Tradeset::get() const { return trades_; }

}
