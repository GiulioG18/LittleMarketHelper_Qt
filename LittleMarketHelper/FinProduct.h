
// ========================================================================
//		Financial product
// ========================================================================

#pragma once

#include <set>
#include <string>
#include <memory>

#include "Null.h"
#include "Observable.h"


namespace lmh {

	// Forward declarations
	class MakeFinProduct;


	class FinProduct : public Observable
	{
		// Every non-const method has to validate
		// quantity and/or price before sending notifications

	public:

		FinProduct(
			const std::string& name,
			const std::string& isin,
			int quantity,
			float price
		);

		float openPosition() const;

		// Non-const methods
		void setQuantityTo(int value);
		void setPriceTo(float value);

		// Const methods
		inline const std::string& name() const;
		inline int quantity() const;
		inline float price() const;

	private:
		
		const std::string name_;
		const std::string isin_;
		int quantity_;
		float price_;
	};


	// Helper class to build a FinProduct

	class MakeFinProduct
	{
	public:
		
		MakeFinProduct(
			const std::string& name,
			const std::string& isin = "",
			int quantity = Null<int>(),
			float price = Null<float>()
		);

		// Return a constructed FinProduct by Value or Shared Pointer
		operator FinProduct() const;
		operator std::shared_ptr<FinProduct>() const;

		// Optionals

		inline MakeFinProduct& withIsin(const std::string& isin);
		inline MakeFinProduct& withQuantity(int quantity);
		inline MakeFinProduct& withPrice(float price);

	private:

		// Throws, user input is assumed to have been checked already
		void validate() const;

	private:

		std::string name_;
		std::string isin_;
		int quantity_;
		float price_;
	};


	// Inline definitions

	inline const std::string& FinProduct::name() const { return name_; }
	inline int FinProduct::quantity() const { return quantity_; }
	inline float FinProduct::price() const { return price_; }

	inline void FinProduct::setQuantityTo(int value)
	{
		if (quantity_ != value)
		{
			quantity_ = value;
			notifyObservers();
		}
	}

	inline void FinProduct::setPriceTo(float value)
	{
		if (price_ != value)
		{
			price_ = value;
			notifyObservers();
		}
	}

	inline MakeFinProduct& MakeFinProduct::withIsin(const std::string& isin)
	{
		isin_ = isin;
		return *this;
	}

	inline MakeFinProduct& MakeFinProduct::withQuantity(int quantity)
	{
		quantity_ = quantity;
		return *this;
	}

	inline MakeFinProduct& MakeFinProduct::withPrice(float price)
	{
		price_ = price;
		return *this;
	}

}

