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


namespace lmh {

	class Calibrator : public Observer
	{
	public:

		using WeightsMap = std::map<std::string, float>; // Maps each security to an ideal weight

	public:

		Calibrator(const Portfolio& portfolio);

		// Non-const method
		// Input keys must match exactly the portfolio trades name.
		// If 'investment' is not provided, the balance from the tracked
		// portfolio object is used instead
		void runOptimization(WeightsMap wm, float investment = 0.0f);

		// Getters
		inline const std::optional<CalibrationResult>& result() const;

	private:

		// Non-const methods
		virtual void update() override;
		void clearResult();
		void setResult(CalibrationResult&& result);

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