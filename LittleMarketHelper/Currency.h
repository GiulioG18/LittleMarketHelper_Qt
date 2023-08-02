// ========================================================================
//		Currencies available
// ========================================================================

#pragma once


namespace lmh {

	class Currency
	{
	public:		

		enum class Iso
		{
			EUR,
			USD,
			GBP,
		};

		Currency(Currency::Iso ccy)
			: ccy_(ccy)
		{
		}

	private:	

		Currency::Iso ccy_;
	};

}
