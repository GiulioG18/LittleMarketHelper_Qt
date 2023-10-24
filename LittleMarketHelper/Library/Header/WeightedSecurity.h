
// ========================================================================
//		Security in a portfolio context
// ========================================================================

#pragma once

#include "Security.h"
#include "Weight.h"


namespace lmh {

	class WSecurity : public Security
	{
	public:

		struct Comp;

	public:

		WSecurity(const Security& security, const std::shared_ptr<Balance>& op);

		// Getters
		inline const Weight& weight() const;

	private:

		Weight weight_;
	};


	// This comparison functor specifies the security key (ISIN) of an STL container
	struct WSecurity::Comp
	{
		// Transparent comparator
		using is_transparent = void;

		bool operator()(const std::shared_ptr<WSecurity>	first,	const std::shared_ptr<WSecurity>&	second)	const;
		bool operator()(const std::string&					first,	const std::shared_ptr<WSecurity>&	second)	const;
		bool operator()(const std::shared_ptr<WSecurity>&	first,	const std::string&					second)	const;
	};


	// Inline definitions
	inline const Weight& WSecurity::weight() const { return weight_; }

}
