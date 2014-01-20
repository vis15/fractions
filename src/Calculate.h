/*
 * Calculate.h
 *
 *  Created on: Nov 11, 2013
 *      Author: dis
 */

#ifndef CALCULATE_H_
#define CALCULATE_H_

#include "parser.h"
#include "util.h"

namespace Math
{

class Calculate
{
public:
	Calculate(constr& expression, bool fractions_enabled, bool debug = false);
	virtual ~Calculate();
	constr calculate();
	constr calcRPN(cvstr& exp);
	constr calcSymbol(constr& symbol, constr& firstnum, constr& secondnum);
	constr add(constr& firstnum, constr& secondnum);
	constr subtract(constr& firstnum, constr& secondnum);
	constr multiply(constr& firstnum, constr& secondnum);
	constr divide(constr& firstnum, constr& secondnum);
	constr power(constr& firstnum, constr& secondnum);
	clongd toStold(constr& num);
	
private:
	void debugOutput(constr& text, FunctionDebug function);
	
	constr exp_;
	bool fractions_enabled_;
	bool debug_;
	
public:
	sigc::signal<void, constr&, const MessageState&, const Verbosity&> signal_say;
	sigc::signal<void, constr&, FunctionDebug> signal_debugoutput;
};

} /* namespace Math */

#endif /* CALCULATE_H_ */
