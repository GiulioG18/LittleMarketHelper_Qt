
// ========================================================================
//		Financial security
// ========================================================================

#pragma once

#include <string>
#include <optional>

#include "Patterns/Observable.h"
#include "Currency.h"


namespace lmh {

	class Security : public Observable
	{
	public:

		Security(
			const std::string& isin,
			const std::string& name,
			Currency ccy,
			int quantity,
			float price
		);

		float openPosition() const;

		// Input validators
		static bool validateIsin(const std::string& isin);
		static bool validateQuantity(int quantity);
		static bool validatePrice(float price);

		// getters 
		inline const std::string& isin() const;
		inline const std::string& name() const;
		inline const Currency ccy() const;
		inline const int quantity() const;
		inline const float price() const;
					
		// Setters
		void setName(const std::string& name);
		void setCcy(Currency ccy);
		void setQuantity(int quantity);
		void setPrice(float price);

	private:
		
		const std::string isin_;	// Key
		std::string name_;
		Currency ccy_;
		int quantity_;
		float price_;
	};


	// Inline definitions
	inline const std::string& Security::isin() const { return isin_; }
	inline const std::string& Security::name() const { return name_; }
	inline const Currency Security::ccy() const { return ccy_; }
	inline const int Security::quantity() const { return quantity_; };
	inline const float Security::price() const { return price_; };
}

