
// ========================================================================
//		Financial security
// ========================================================================

#pragma once

#include <string>

#include "Patterns/Observable.h"
#include "Quote.h"


namespace lmh {

	class Security : public Observable
	{
	public:

		// Constructor [ MAY THROW ]
		Security( 
			const std::string& isin,
			const std::string& name,
			int quantity,
			const Quote& quote
		);

		Price marketValue() const;

		// Input validators
		static bool validateIsin(const std::string& isin);
		static bool validateQuantity(int quantity);

		// getters 
		inline const std::string& isin() const;
		inline const std::string& name() const;
		inline const int quantity() const;
		inline const Quote& quote() const;
					
		// Setters [ MAY THROW ]
		void setName(const std::string& name);
		void setQuantity(int quantity);
		void setQuote(double value);

	private:
		
		const std::string isin_;	// Key
		std::string name_;
		int quantity_;
		Quote quote_;
	};


	// Inline definitions
	inline const std::string& Security::isin() const { return isin_; }
	inline const std::string& Security::name() const { return name_; }
	inline const int Security::quantity() const { return quantity_; };
	inline const Quote& Security::quote() const { return quote_; };
}

