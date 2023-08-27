
#include <algorithm>
#include <memory>

#include "Calibrator.h"

//#include "ortools/linear_solver/linear_solver.h"
//
//
//using namespace operations_research;
//
// Example:
// 
// Balance:			1200$
// Product 1:		p1 = 100,		w1 = 0.3,		a1 = w1 * Balance,		Q1 = ?
// Product 2:		p2 = 25,		w2 = 0.7,		a2 = w1 * Balance,		Q2 = ?
// 
// Objective function [minimize]:
//		abs(p1Q1 - a1) + abs(p2Q2 - a2)			
// 
// Transformed objective function to accomodate abs [minimize]: (see: https://lpsolve.sourceforge.net/5.1/absolute.htm)
//		X1 + X2
//
// Constraints:
//		c1: p1Q1 + p2Q2 < Balance
//		c2: Q1 < Balance / p1		[not strictly needed, but useful for faster convergence]
//		c3: Q2 < Balance / p2		[not strictly needed, but useful for faster convergence]
//		c4: +(p1Q1 - a1) <= X1
//		c5: -(p1Q1 - a1) <= X1
//		c6: +(p2Q2 - a2) <= X2
//		c7: -(p2Q2 - a2) <= X2
// 

namespace lmh {

	Calibrator::Calibrator(const Portfolio& portfolio)
		: 
		result_(std::nullopt),
		trades_(portfolio.included()),
		balance_(portfolio.balance())
	{
		REQUIRE(trades_, "invalid trades");
		REQUIRE(balance_, "invalid balance");

		registerWith(portfolio.balance());
	}

	void Calibrator::runOptimization(WeightsMap wm, float investment)
	{
		// Clear old output
		clearResult();

		if (!validateWeightsMap(wm))
			return;

		// If investment amount is not provided, use the linked portfolio 
		// balance as cash constraint for the optimization
		float inv = Null<float>::check(investment) ?
			balance_->value() :
			investment;		
		if (inv <= 0.0f)
			return;

		CalibrationResult result;
		bool successful= result.initialize(
			wm, 
			inv,
			trades_.get());

		if (!successful)
			return;

		// Run all available optimizations until successful
		successful = opt_SCIP(result);
		if (!successful)
			successful = opt_Floored(result);

		if (successful)
			setResult(std::move(result));
	}

	void Calibrator::update()
	{
		clearResult();
	}

	void Calibrator::clearResult()
	{
		result_ = std::nullopt;
	}

	void Calibrator::setResult(CalibrationResult&& result)
	{
		result_ = std::make_optional(std::move(result));
	}

	bool Calibrator::opt_SCIP(CalibrationResult& result)
	{
		// TODO: implement
		 
		//// Create solver
		//std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP_MIXED_INTEGER_PROGRAMMING"));
		//if (!solver)
		//	return EXIT_FAILURE;

		//// Create constants
		//const float balance = 1200.0f;
		//const float p1 = 100.0f;
		//const float p2 = 25.0f;
		//const float w1 = 0.3f;
		//const float w2 = 0.7f;
		//const float a1 = w1 * static_cast<float>(balance);
		//const float a2 = w2 * static_cast<float>(balance);

		//// Create variables
		//MPVariable* const Q1 = solver->MakeIntVar(0.0, balance / p1, "Q1");
		//MPVariable* const Q2 = solver->MakeIntVar(0.0, balance / p2, "Q2");
		//MPVariable* const X1 = solver->MakeNumVar(0.0, INFINITY, "X1");
		//MPVariable* const X2 = solver->MakeNumVar(0.0, INFINITY, "X2");

		//// Helpers
		//LinearExpr Q1expr(Q1);
		//LinearExpr Q2expr(Q2);
		//const LinearRange l4 = +1 * (p1 * Q1expr - a1) <= X1;
		//const LinearRange l5 = -1 * (p1 * Q1expr - a1) <= X1;
		//const LinearRange l6 = +1 * (p2 * Q2expr - a2) <= X2;
		//const LinearRange l7 = -1 * (p2 * Q2expr - a2) <= X2;

		//// Create constraints	(NB: c2 and c3 are included in variable definitions)
		//MPConstraint* const c1 = solver->MakeRowConstraint(0.0, balance, "c1");
		//c1->SetCoefficient(Q1, p1);
		//c1->SetCoefficient(Q2, p2);
		//solver->MakeRowConstraint(l4);
		//solver->MakeRowConstraint(l5);
		//solver->MakeRowConstraint(l6);
		//solver->MakeRowConstraint(l7);

		//// Create objective function
		//MPObjective* const objective = solver->MutableObjective();
		//objective->MinimizeLinearExpr(LinearExpr(X1) + LinearExpr(X2));

		//MPSolver::ResultStatus status = solver->Solve();
		//if (status == MPSolver::OPTIMAL) {
		//	std::cout << "Optimal Solution Found:" << std::endl;
		//	std::cout << "Buy " << Q1->solution_value() << " units of Product 1" << std::endl;
		//	std::cout << "Buy " << Q2->solution_value() << " units of Product 2" << std::endl;
		//}
		//else {
		//	std::cout << "No solution found." << std::endl;
		//}

		bool successful = result.validate();
		if (!successful)
			result.partialReset();

		return successful;
	}

	bool Calibrator::opt_Floored(CalibrationResult& result)
	{
		// This algorithm computes the ideal quantity by simply
		// flooring the floating computation.
		// This solution has seemingly adequate results but tends
		// to underuse the investment available

		const float&	investment = result.investment_;
		float&			cash = result.cash_;
		float&			openPosition = result.openPosition_;

		// Fill ideal quantities
		std::for_each(std::begin(result.data_), std::end(result.data_), 
			[&investment](auto& d)
			{
				d.idealQuantity_ = static_cast<int>(std::floorf(d.idealWeight_ * investment / d.price_));
			}
		);

		// Fill real quantities
		openPosition = 0.0f;
		std::for_each(std::begin(result.data_), std::end(result.data_), 
			[&openPosition, investment](auto& d)
			{
				// If there is not enough cash for this product, we only use the remaining cash
				if (openPosition + d.idealQuantity_ * d.price_ > investment)
				{
					d.realQuantity_ = static_cast<int>(std::floorf((investment - openPosition) / d.price_));
					
				}
				else
				{
					d.realQuantity_ = d.idealQuantity_;
				}

				openPosition += d.realQuantity_ * d.price_;
			}
		);
		REQUIRE(openPosition > 0.0f, "open position is not positive");

		// Fill real weights
		std::for_each(std::begin(result.data_), std::end(result.data_), 
			[openPosition](auto& d)
			{
				d.realWeight_ = d.realQuantity_ * d.price_ / openPosition;
			}
		);

		// Cash
		cash = investment - openPosition;

		bool successful = result.validate();
		if (!successful)
			result.partialReset();

		return successful;
	}

	bool Calibrator::validateWeightsMap(const WeightsMap& wm) const
	{
		// Check if input weights match portfolio trades 
		if (wm.size() != trades_->get().size())
			return false;

		float weightSum = 0.0f;
		for (const auto& trade : trades_->get())
		{
			// Check that it is provided 1 weight per product
			std::string name = trade.first->name();
			auto it = wm.find(name);
			if (it == wm.end())
				return false;

			// Check ideal weights
			const float& weight = it->second;
			if (weight > 1 || weight < 0)
				return false;
			weightSum += weight;
		}

		// Check if sum of ideal weights is 1.0
		if (weightSum - 1.0f > FLT_EPSILON)
			return false;

		return true;
	}
}