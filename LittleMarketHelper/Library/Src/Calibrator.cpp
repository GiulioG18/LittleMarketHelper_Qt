
#include <algorithm>

#include "Calibrator.h"


#include <memory>

#include "ortools/linear_solver/linear_solver.h"


using namespace operations_research;

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

int sblech() 
{
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
	return 2;
}


namespace lmh {

	Calibrator::Calibrator(const Portfolio& portfolio)
		: 
		output_(std::nullopt),
		trades_(portfolio.included()),
		balance_(portfolio.balance())
	{
		REQUIRE(trades_, "invalid trades");
		REQUIRE(balance_, "invalid balance");

		registerWith(portfolio.balance());
	}

	bool Calibrator::runOptimization(Input input, float amountToInvest)
	{
		// Clear old output
		output_ = std::nullopt;

		if (!validateInput(input))
			return false;


		float total = Null<float>::check(amountToInvest) ?
			balance_->value() :	// Use the balance as total
			amountToInvest;		// Use the customized amount as total
		if (total <= 0.0f)
			return false;

		bool successful = true;

		// Start optimization
		
		sblech();

		if (true/*!successful*/)
			return NaiveFallback(input, total);
	}

	const Calibrator::Output& Calibrator::output() const
	{
		return output_;
	}

	void Calibrator::update()
	{
		output_ = std::nullopt;
	}

	bool Calibrator::NaiveFallback(const Input& input, float amountToInvest)
	{
		//// Fill data from input
		//std::vector<ODatum> optData;
		//for (const auto& i : input)
		//{
		//	static int counter = 0;

		//	int index = counter++;
		//	std::string name = i.first;
		//	float price = i.second.price_;
		//	float idealWeight = i.second.targetWeight_;
		//	float realWeight = Null<float>();		// To be computed
		//	int idealQuantity = Null<int>();		// To be computed
		//	int realQuantity = Null<int>();			// To be computed

		//	optData.push_back(ODatum(index, name, price, idealWeight, realWeight, idealQuantity, realQuantity));
		//}
		//// Sort based on price.
		//// This actually should not have much of an impact, since
		//// we are flooring the ideal quantity (flooring will cause to 
		//// under-invest the available amount, but allows enough cash for all products
		//std::sort(std::begin(optData), std::end(optData), [](const ODatum& i, const ODatum& j)
		//	{
		//		return i.price_ > j.price_;
		//	}
		//);

		//// Fill ideal quantities
		//std::for_each(std::begin(optData), std::end(optData), [amountToInvest](ODatum& o)
		//	{
		//		o.idealQuantity_ = static_cast<int>(std::floorf(o.idealWeight_ * amountToInvest / o.price_));
		//	}
		//);

		//// Fill real quantities
		//float total = 0.0f;
		//std::for_each(std::begin(optData), std::end(optData), [&total, amountToInvest](ODatum& o)
		//	{
		//		float newTotal = total + o.idealQuantity * o.price;

		//		if (newTotal > amountToInvest)
		//		{
		//			o.realQuantity = static_cast<int>(std::floorf((amountToInvest - total) / o.price));
		//		}
		//		else
		//		{
		//			o.realQuantity = o.idealQuantity;
		//		}

		//		total = newTotal;
		//	}
		//);

		//// Fill real weights
		//std::for_each(std::begin(optData), std::end(optData), [total](OptDatum_& o)
		//	{
		//		o.realWeight = o.realQuantity * o.price / total;
		//	}
		//);

		//// Validate results
		//bool successful = true;
		//std::for_each(std::begin(optData), std::end(optData), [&successful](OptDatum_& o)
		//	{
		//		successful =
		//				(!Null<float>::check(o.realWeight) &&
		//				!Null<int>::check(o.idealQuantity) &&
		//				!Null<int>::check(o.realQuantity));
		//	}
		//);

		//return successful;
		return false;
	}

	bool Calibrator::validateInput(Input& input)
	{
		//if (input.size() != trades_->trades().size())
		//	return false;

		//float weightSum = 0.0f;
		//for (const auto& trade : trades_->trades())
		//{
		//	// Check if input products match portfolio trades
		//	std::string name = trade.first->name();
		//	auto it = input.find(name);
		//	if (it == input.end())
		//		return false;

		//	// Extract price
		//	it->second.price_ = trade.first->price();
		//	REQUIRE(!Null<float>::check(it->second.price_), "null price");

		//	// Validate target weights
		//	const float& weight = it->second.targetWeight_;
		//	if (weight > 1 || weight < 0)
		//		return false;
		//	weightSum += weight;
		//}
		//// Check if sum of target weights is 1
		//if (weightSum - 1.0f > FLT_EPSILON)
		//	return false;

		//// Check balance
		//if (balance_->value() <= 0)
		//	return false;

		return true;
	}

}