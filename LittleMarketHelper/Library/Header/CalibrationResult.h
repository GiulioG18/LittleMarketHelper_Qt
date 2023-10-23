// ========================================================================
//		Calibration results
// ========================================================================

#pragma once

#include <vector>
#include <string>
#include <map>

#include "Currency.h"
#include "WeightedSecurity.h"


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

		//								 isin	 weight
		using WeightsMap = std::map<std::string, double>;

	public:

		CalibrationResult(Currency::Type ccy);

		// Non-const methods
		// Assumes that all the inputs are validated before, otherwise its behaviour is undefined
		bool initialize(const WeightsMap& wm, double investment, const std::set<std::shared_ptr<WSecurity>, WSecurity::Comp>& securities);
		void partialReset();

		// Const methods
		bool validate() const;

		// Getters
		inline const auto& data() const;
		inline double investment() const;
		inline double cash() const;
		inline double openPosition() const;
		inline bool initialized() const;

	private:

		struct Datum;

		std::vector<Datum> data_;
		Currency::Type ccy_;
		double investment_;
		double openPosition_;	
		double cash_; // Investment - open position
		bool initialized_;
	};


	// Inline definitions
	inline const auto& CalibrationResult::data() const { return data_; }
	inline double CalibrationResult::investment() const { return investment_; }
	inline double CalibrationResult::cash() const { return cash_; }
	inline double CalibrationResult::openPosition() const { return openPosition_; }
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
			double price,
			double idealWeight,
			int idealQuantity,
			double realWeight,
			int realQuantity);

		// Getters
		inline std::string name() const;
		inline double price() const;
		inline double idealWeight() const;
		inline int idealQuantity() const;
		inline double realWeight() const;
		inline int realQuantity() const;

	private:

		// Const methods
		bool validate() const;

		// Non-const methods
		void partialReset();	// Reset non-const values
		

	private:

		// Input
		const std::string	name_;
		const double		price_;
		const double		idealWeight_;
		// Output
		int				idealQuantity_;
		double			realWeight_;
		int				realQuantity_;
	};


	// Inline definitions
	inline std::string CalibrationResult::Datum::name() const { return name_; };
	inline double CalibrationResult::Datum::price() const { return price_; };
	inline double CalibrationResult::Datum::idealWeight() const { return idealWeight_; };
	inline int CalibrationResult::Datum::idealQuantity() const { return idealQuantity_; };
	inline double CalibrationResult::Datum::realWeight() const { return realWeight_; };
	inline int CalibrationResult::Datum::realQuantity() const { return realQuantity_; };
}

