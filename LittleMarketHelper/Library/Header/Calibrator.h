// ========================================================================
//		Portfolio weights calibrator
// ========================================================================

#pragma once

#include <memory>
#include <map>

#include "Patterns/Observable.h"
#include "CalibrationResult.h"
#include "Portfolio.h"
#include "Weight.h"


namespace lmh {

	class Calibrator : public Observer
	{
	public:

		// Maps each security to an ideal weight
		using WeightsMap = std::map<std::string, double>; 

	public:

		Calibrator(const Portfolio* const portfolio);

		// Non-const method
		// Input keys must match exactly all portfolio securities
		void runOptimization(WeightsMap wm, Currency Currency, double investment);

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
		const Portfolio* portfolio_;
		//std::shared_ptr<Tradeset> trades_;
		//std::shared_ptr<Balance> balance_; // Observed
	};	


	// Inline definitions

	const std::optional<CalibrationResult>& Calibrator::result() const { return result_; }

}