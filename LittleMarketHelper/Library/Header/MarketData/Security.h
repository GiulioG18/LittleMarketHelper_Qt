
// ========================================================================
//		Financial security
// ========================================================================

#pragma once

#include <string>

#include "MarketData/Isin.h"
#include "MarketData/StockExchange.h"


/*
#include <string>

#include "Patterns/Observable.h"
#include "Quote.h"

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
*/

namespace lmh {


	class Security
	{
	public:

		using Symbol = std::string;

	public:

		Security(Isin isin);

		Security(
			Isin isin,
			std::string name,
			std::string description,
			Symbol symbol,
			StockExchange stockExchange
		);

		// Operators
		bool operator<(const Security& other) const;

	private:

		Isin isin_;
		std::string name_;
		std::string description_;
		Symbol symbol_;
		StockExchange stockExchange_;
	};
	
}

