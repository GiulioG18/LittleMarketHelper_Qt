// ========================================================================
//		Calibration results
// ========================================================================

#pragma once

#include <vector>
#include <string>
#include <map>

#include "Tradeset.h"


namespace lmh {

	// Forward declarations
	class Optimizer;
	class SCIP;
	class Floored;


	class CalibrationResult
	{
	public:

		friend class Optimizer;
		friend class SCIP;
		friend class Floored;

		//							   Isin		 weight
		using WeightsMap = std::map<std::string, float>;

	public:

		CalibrationResult() = default;

		// Non-const methods
		bool initialize(const WeightsMap& wm, float investment, Tradeset* tradeset);
		void partialReset();

		// Const methods
		bool validate() const;

		// Getters
		inline const auto& data() const;
		inline float investment() const;
		inline float cash() const;
		inline float openPosition() const;
		inline bool initialized() const;

	private:

		struct Datum;

		std::vector<Datum> data_;
		float investment_	= 0.0f;
		float cash_			= 0.0f;	// Investment - open position
		float openPosition_ = 0.0f;	
		bool initialized_	= false;
	};


	// Inline definitions
	inline const auto& CalibrationResult::data() const { return data_; }
	inline float CalibrationResult::investment() const { return investment_; }
	inline float CalibrationResult::cash() const { return cash_; }
	inline float CalibrationResult::openPosition() const { return openPosition_; }
	inline bool CalibrationResult::initialized() const { return initialized_; }



	struct CalibrationResult::Datum
	{
	public:

		friend class CalibrationResult;
		friend class SCIP;
		friend class Floored;

	public:

		Datum(
			std::string name,
			float price,
			float idealWeight,
			int idealQuantity,
			float realWeight,
			int realQuantity);

		// Getters
		inline std::string name() const;
		inline float price() const;
		inline float idealWeight() const;
		inline int idealQuantity() const;
		inline float realWeight() const;
		inline int realQuantity() const;

	private:

		// Const methods
		bool validate() const;

		// Non-const methods
		void partialReset();	// Reset non-const values
		

	private:

		// Input
		const std::string	name_;
		const float			price_;
		const float			idealWeight_;
		// Output
		int				idealQuantity_;
		float			realWeight_;
		int				realQuantity_;
	};


	// Inline definitions
	inline std::string CalibrationResult::Datum::name() const { return name_; };
	inline float CalibrationResult::Datum::price() const { return price_; };
	inline float CalibrationResult::Datum::idealWeight() const { return idealWeight_; };
	inline int CalibrationResult::Datum::idealQuantity() const { return idealQuantity_; };
	inline float CalibrationResult::Datum::realWeight() const { return realWeight_; };
	inline int CalibrationResult::Datum::realQuantity() const { return realQuantity_; };
}

