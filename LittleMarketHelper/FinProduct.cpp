
#include "FinProduct.h"
#include "Assertions.h"


namespace lmh {

	FinProduct::FinProduct(
		const std::string& name,
		const std::string& isin,
		int quantity,
		float price
	)
		: name_(name), isin_(isin), quantity_(quantity), price_(price)
	{
	}

	float lmh::FinProduct::openPosition() const
	{
		return static_cast<float>(quantity_) * price_;
	}



	MakeFinProduct::MakeFinProduct(
		const std::string& name,
		const std::string& isin,
		int quantity,
		float price
	)
		: name_(name), isin_(isin), quantity_(quantity), price_(price)
	{
	}

	MakeFinProduct::operator FinProduct() const
	{
		validate();
		FinProduct product{
			name_,
			isin_,
			quantity_,
			price_
		};

		return product;
	}

	MakeFinProduct::operator std::shared_ptr<FinProduct>() const
	{
		validate();
		std::shared_ptr<FinProduct> product = std::make_shared<FinProduct>(
			name_,
			isin_,
			quantity_,
			price_
			);

		return product;
	}

	void MakeFinProduct::validate() const
	{
		VALIDATE_USER_INPUT(!name_.empty(), "name is empty");
		// ISIN is a unique 12-character alphanumeric code 
		// NB: optional, but when provided needs the have the correct length
		VALIDATE_USER_INPUT(isin_.empty() || isin_.length() == 12, "ISIN incorrect");
		VALIDATE_USER_INPUT(quantity_ >= 0, "negative quantity");
		VALIDATE_USER_INPUT(price_ > 0, "non-positive price");
	}

}