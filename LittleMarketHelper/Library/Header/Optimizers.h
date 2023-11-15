
// ========================================================================
//		Optimization algorithms
// ========================================================================

#pragma once

#include "CalibrationResult.h"


namespace lmh {

	class Optimizer
	{
	public:

		Optimizer(CalibrationResult& result);
		virtual ~Optimizer() = default;

		bool run();

	private:

		virtual void optimize() = 0;

	protected:

		CalibrationResult& result_;
	};


	class SCIP : public Optimizer
	{
	public:

		SCIP(CalibrationResult& result)
			: Optimizer(result)
		{
		}

	private:

		virtual void optimize();
	};


	class Floored : public Optimizer
	{
	public:

		Floored(CalibrationResult& result)
			: Optimizer(result)
		{
		}

	private:

		virtual void optimize();
	};

}