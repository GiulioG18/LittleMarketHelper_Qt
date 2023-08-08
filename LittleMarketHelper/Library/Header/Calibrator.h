// ========================================================================
//		Portfolio weights calibrator
// ========================================================================

#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <map>

#include "Portfolio.h"
#include "Weight.h"
#include "Patterns/Observable.h"
#include "Utils/Null.h"


namespace lmh {

	class Calibrator : public Observer
	{
		// The output of the optimization is an object that
		// maps each product to the ideal quantity to hold

	public:

		struct Datum
		{
		public:

			friend class Calibrator;

		public:

			// price_ is extracted later during the calibration,
			// Users must only specify the desired target weight
			Datum(float targetWeight)
				: 
				targetWeight_(targetWeight),
				price_(Null<float>())
			{
			}

		private:

			const float targetWeight_;
			float price_;
		};

	public:

		using Input = std::map<std::string, Datum>;
		using Output = std::optional<std::map<std::string, int>>;

	public:

		Calibrator(const Portfolio& portfolio);

		// Non-const method
		// Input products must match exactly the portfolio trades
		bool runOptimization(Input input);

		// Const methods
		const Output& output() const;

	private:

		// Non-const methods
		virtual void update() override;
		// Get input from included trades in the portfolio
		bool validateInput(Input& input);

	private:

		Output output_;
		std::shared_ptr<Tradeset> trades_;
		// Observed
		std::shared_ptr<Balance> balance_;
	};	

}