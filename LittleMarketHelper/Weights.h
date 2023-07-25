
// ========================================================================
//		Product weights
// ========================================================================

#pragma once

#include "Null.h"


namespace lmh {

	class Weights
	{
	public:

		Weights()
			: 
			actual_(Null<float>()),
			desired_(Null<float>()),
			estimated_(Null<float>())
		{
		}

	private:

		float actual_;		// Current weight
		float desired_;		// User selected (for calibration)
		float estimated_;	// Potential weight once portfolio is recalibrated
	};

}
