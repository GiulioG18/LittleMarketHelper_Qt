
// ========================================================================
//		Financial security
// ========================================================================

#pragma once

#include <string>

#include "Patterns/Observable.h"
#include "Quote.h"


namespace lmh {

	// Forward declarations
	class SecurityShell;

	class Security : public Observable
	{
	public:

		Security( // [ MAY THROW ]
			std::string_view isin,
			std::string_view name,
			uint32_t quantity,
			const Quote& quote
		); 

		Security( // [ MAY THROW ]
			const SecurityShell& shell,
			uint32_t quantity,
			const Quote& quote
		); 

		virtual ~Security() = default;

		Price marketValue() const;

		// Input validators
		static bool validateIsin(std::string_view isin);
		static bool validateQuantity(uint32_t quantity);

		// Getters 
		inline const std::string& isin() const;
		inline const std::string& name() const;
		inline uint32_t quantity() const;
		inline const Quote& quote() const;
					
		// Setters
		void setName(std::string_view name);
		void setQuantity(uint32_t quantity); // [ MAY THROW ]
		void setQuote(double amount); // [ MAY THROW ]

	private:
		
		const std::string isin_; // Key
		std::string name_;
		uint32_t quantity_;
		Quote quote_;
	};


	// Inline definitions
	inline const std::string& Security::isin() const { return isin_; }
	inline const std::string& Security::name() const { return name_; }
	inline uint32_t Security::quantity() const { return quantity_; };
	inline const Quote& Security::quote() const { return quote_; };
}

