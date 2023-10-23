
// ========================================================================
//		Security quote
// ========================================================================

#pragma once

#include "Utils/Assertions.h"
#include "Http/ConfigRequest.h"
#include "Price.h"

// TODO: move definitions to cpp file

namespace lmh {

	// Forward declarations
	class Security;


	class Quote 
	{
	public:

		// Constructor [ MAY THROW ]
		Quote(const Price& price, double timestamp)
			:
			price_(price),
			timestamp_(timestamp)
		{
		}		

		// Setters [ MAY THROW ]
		void setValue(double amount)
		{
			price_.set(amount);
		}

		// Getters
		const Price& price() const
		{
			return price_;
		}

		double timestamp() const
		{
			return timestamp_;
		}

	private:

		Price price_;
		double timestamp_; // Const // TODO: Find appropriate type
	};

}