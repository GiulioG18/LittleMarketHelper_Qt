// ========================================================================
//		Portfolio weights calibrator
// ========================================================================

#pragma once

#include <memory>
#include <map>

#include "Portfolio.h"
#include "CalibrationResult.h"
#include "Weight.h"
#include "Patterns/Observable.h"
#include "Utils/Null.h"


namespace lmh {

	class Calibrator : public Observer
	{
	public:

		using WeightsMap = std::map<std::string, float>; // Maps each product to an ideal weight

	public:

		Calibrator(const Portfolio& portfolio);

		// Non-const method
		// Input keys must match exactly the portfolio trades name.
		// If 'investment' is not provided, the balance from the tracked
		// portfolio object is used instead
		void runOptimization(WeightsMap wm, float investment = Null<float>());

		// Getters
		inline const std::optional<CalibrationResult>& result() const;

	private:

		// Non-const methods
		virtual void update() override;
		void clearResult();
		void setResult(CalibrationResult&& result);

		// Optimizations available:
		// NB: every optmization must (partially) reset the result
		// object in case it is not successful
		bool opt_SCIP(CalibrationResult& result);
		bool opt_Floored(CalibrationResult& result);

		// Const methods
		bool validateWeightsMap(const WeightsMap& wm) const;

	private:

		std::optional<CalibrationResult> result_;
		std::shared_ptr<Tradeset> trades_;
		// Observed
		std::shared_ptr<Balance> balance_;
	};	


	// Inline definitions

	const std::optional<CalibrationResult>& Calibrator::result() const { return result_; }

}