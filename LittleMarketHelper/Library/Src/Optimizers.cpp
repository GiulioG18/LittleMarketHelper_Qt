
#include <algorithm>

#include "Optimizers.h"
#include "Utils/Assertions.h"


namespace lmh {

	Optimizer::Optimizer(CalibrationResult& result)
		: result_(result)
	{
		ENSURE(result_.initialized(), "result are not initialized");
	}

	bool Optimizer::run()
	{
		this->optimize();
		bool successful = result_.validate();
		if (!successful)
			result_.partialReset();

		return successful;
	}



	// Optimization algorithms

	void SCIP::optimize()
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
	}

	void Floored::optimize()
	{
		// This algorithm computes the ideal quantity by simply
		// flooring the floating computation.
		// This solution has seemingly adequate results but tends
		// to underuse the investment available

		const float& investment = result_.investment();
		float& cash = result_.cash_;
		float& openPosition = result_.openPosition_;

		// Fill ideal quantities
		std::for_each(std::begin(result_.data_), std::end(result_.data_),
			[&investment](auto& d)
			{
				d.idealQuantity_ = static_cast<int>(std::floorf(d.idealWeight_ * investment / d.price_));
			}
		);

		// Fill real quantities
		openPosition = 0.0f;
		std::for_each(std::begin(result_.data_), std::end(result_.data_),
			[&openPosition, investment](auto& d)
			{
				// If there is not enough cash for this security, we only use the remaining cash
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
		std::for_each(std::begin(result_.data_), std::end(result_.data_),
			[openPosition](auto& d)
			{
				d.realWeight_ = d.realQuantity_ * d.price_ / openPosition;
			}
		);

		// Cash
		result_.cash_ = investment - openPosition;
	}

}
