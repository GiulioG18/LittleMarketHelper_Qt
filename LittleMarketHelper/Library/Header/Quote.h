
// ========================================================================
//		Quote
// ========================================================================

#pragma once

#include "MarketValue.h"
#include "Utils/Chrono.h"
#include "MarketData/DatasetAccess.h"


namespace lmh {

	// Forward declarations
	class Isin;

	/*
	class Quote
	{
	public:

		Quote(double price, Currency currency); // [ MAY THROW ]

		// Getters
		inline double price() const;
		inline Currency currency() const;

		// Setters
		void set(double price); // [ MAY THROW ]

		// Comparison operators
		inline auto operator<=>(const Quote& other) const;
		bool operator==(const Quote& other) const;

		// Arithmetic operators // TODO: add missing common operators
		Quote operator+(const Quote& other) const;
		Quote operator/(const Quote& other) const;
		Quote operator*(const Quote& other) const;
		Quote& operator+=(const Quote& other);
		Quote operator+(double price) const;
		Quote operator/(double price) const;
		Quote operator*(double quantity) const;
		Quote& operator+=(double price);

	private:

		double price_;
		Currency currency_;
	};


	// Inline definitions
	inline double Quote::price() const { return price_; }
	inline Currency Quote::currency() const { return currency_; }


	// Template definitions

	inline auto Quote::operator<=>(const Quote& other) const
	{
		Quote convertedOther = Forex::convertQuote(other, this->currency_);
		return this->price_ <=> convertedOther.price_;
	}
	*/


	class Quote
	{
	private:

		// Hide constructor from user
		Quote(const MarketValue& userMv, const MarketValue& baseMv, const Date& date); // [ MAY THROW ]

	public:

		// Getters
		inline const MarketValue& userMv() const;
		inline const MarketValue& baseMv() const;
		inline const Date& date() const;

	private:

		MarketValue userMv_; // Market value of user requested currency
		MarketValue baseMv_; // Market value of base currency
		Date date_;
	};



	// Inline definitions
	inline const MarketValue& Quote::userMv() const { return userMv_; }
	inline const MarketValue& Quote::baseMv() const { return baseMv_; }
	inline const Date& Quote::date() const { return date_; };
}
