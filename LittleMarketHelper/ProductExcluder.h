
// ========================================================================
//		Product excluder
// ========================================================================

#pragma once


namespace lmh {

	// Wrapper around a bool type to allow for  
	// the object to be Observable

	class ProductExcluder
	{
	public:

		ProductExcluder()
			: exclude_(false)
		{
		}

	private:

		bool exclude_;
	};
}