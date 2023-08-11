// ========================================================================
//		Portfolio weights calibrator
// ========================================================================

#pragma once

#include <memory>
#include <optional>
#include <map>
#include <vector>

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

		// Output
		struct ODatum
		{
		public:

			friend class Calibrator;

		private:

			ODatum(
				int index,
				std::string name,
				float price,
				float idealWeight,
				float realWeight,
				int idealQuantity,
				int realQuantity)
				: 
				index_(index), name_(name), price_(price),
				idealWeight_(idealWeight), realWeight_(realWeight),
				idealQuantity_(idealQuantity), realQuantity_(realQuantity)
			{
			};

		private:

			const int index_;
			const std::string name_;
			const float price_;
			const float idealWeight_;
			float realWeight_;
			int idealQuantity_;
			int realQuantity_;
		};

	public:

		using Input = std::map<std::string, float>;
		using Output = std::optional<std::vector<ODatum>>;

	public:

		Calibrator(const Portfolio& portfolio);

		// Non-const method
		// Input keys must match exactly the portfolio trades name.
		// If 'amountToInvest' is not provided, the balance from the tracked
		// portfolio object is used instead
		bool runOptimization(Input input, float amountToInvest = Null<float>());

		// Const methods
		const Output& output() const;

	private:

		// Non-const methods
		virtual void update() override;
		bool NaiveFallback(const Input& input, float amountToInvest);
		// Get input from included trades in the portfolio
		bool validateInput(Input& input);

	private:

		Output output_;
		std::shared_ptr<Tradeset> trades_;
		// Observed
		std::shared_ptr<Balance> balance_;
	};	

}