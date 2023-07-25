
// ========================================================================
//		Financial product
// ========================================================================

#pragma once

#include <set>
#include <string>

#include "Null.h"
#include "Errors.h"


namespace lmh {

	// Forward declarations
	class MakeFinProduct;



	class FinProduct
	{
	public:

		FinProduct(
			const std::string& name,
			const std::string& isin,
			int quantity,
			float price
		);

	public:

		inline float openPosition() const;

	private:

		const std::string name_;
		std::string isin_;
		int quantity_;
		float price_;
	};



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

	inline float lmh::FinProduct::openPosition() const
	{
		return static_cast<float>(quantity_) * price_;
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

