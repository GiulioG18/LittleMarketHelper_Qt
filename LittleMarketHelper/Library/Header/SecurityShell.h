
// ========================================================================
//		Uncomplete security
// ========================================================================

#pragma once

#include <string>


namespace lmh {

	class SecurityShell
	{
	public:

		SecurityShell(std::string_view isin, std::string_view name);

		// Getters
		inline const std::string& isin() const;
		inline const std::string& name() const;

	private:

		std::string isin_;
		std::string name_;
	};


	// Inline definitions
	inline const std::string& SecurityShell::isin() const { return isin_; }
	inline const std::string& SecurityShell::name() const { return name_; }
}