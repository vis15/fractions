/*
 * Calculate.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: dis
 */

#include <math.h>

#include <stack>
#include <iostream>

#include "Calculate.h"
#include "fraction.h"

namespace Math
{

Calculate::Calculate(constr& expression, bool fractions_enabled, bool debug) : exp_(expression), fractions_enabled_(fractions_enabled), debug_(debug)
{
	// 

}

Calculate::~Calculate()
{
	// 
}

constr Calculate::calculate()
{
	vstr voutput;
	Parser::Parser p(exp_, fractions_enabled_, debug_);
	p.signal_debugoutput.connect(sigc::mem_fun(*this, &Calculate::debugOutput));
	
	try
	{
		voutput = p.parse();
	}
	catch(constr& err)
	{
		throw err;
	}
	
	signal_debugoutput.emit(Util::vecToString(voutput), FunctionDebug::kRPN);
	
	constr output = calcRPN(voutput);
	signal_debugoutput.emit(output, FunctionDebug::kCalc);
	
	return output;
}

void Calculate::debugOutput(constr& text, FunctionDebug function)
{
	signal_debugoutput.emit(text, function);
}

constr Calculate::calcRPN(cvstr& exp)
{
	std::stack<str> stk;
	constr stack_init = kStack_Init();
	str last = "";
	str last2 = "";
	str debugoutput = "";
	
	stk.push(stack_init);
	
	for(uint i = 0; i<exp.size(); i++)
	{
		constr token = exp.at(i);
		
		if(isSymbol(token))
		{
			last = stk.top();
			stk.pop();
			last2 = stk.top();
			stk.pop();
			stk.push(calcSymbol(token, last2, last));
		}
		else
		{
			stk.push(token);
		}
	}
	
	return stk.top();
}

constr Calculate::calcSymbol(constr& symbol, constr& firstnum, constr& secondnum)
{	
	if(symbol == kAdd())
	{
		return add(firstnum, secondnum);
	}
	else if(symbol == kSubtract())
	{
		return subtract(firstnum, secondnum);
	}
	else if(symbol == kMuliply())
	{
		return multiply(firstnum, secondnum);
	}
	else if(symbol == kDivide())
	{
		return divide(firstnum, secondnum);
	}
	else if(symbol == kPower())
	{
		return power(firstnum, secondnum);
	}
	
	return "";
}

constr Calculate::add(constr& firstnum, constr& secondnum)
{	
	bool isff = Fraction::isFraction(firstnum); // IS First Fraction
	bool issf = Fraction::isFraction(secondnum); //IS Second Fraction
	
	if(isff || issf)
	{
		return Fraction(Fraction(firstnum) + Fraction(secondnum)).toString();
	}
	else
	{
		return Util::toString(toStold(firstnum) + toStold(secondnum));
	}
}

constr Calculate::subtract(constr& firstnum, constr& secondnum)
{
	bool isff = Fraction::isFraction(firstnum); // IS First Fraction
	bool issf = Fraction::isFraction(secondnum); //IS Second Fraction
	
	if(isff || issf)
	{
		return Fraction(Fraction(firstnum) - Fraction(secondnum)).toString();
	}
	else
		return Util::toString(toStold(firstnum) - toStold(secondnum));
}

constr Calculate::multiply(constr& firstnum, constr& secondnum)
{
	bool isff = Fraction::isFraction(firstnum); // IS First Fraction
	bool issf = Fraction::isFraction(secondnum); //IS Second Fraction
	
	if(isff || issf)
	{
		return Fraction(Fraction(firstnum) * Fraction(secondnum)).toString();
	}
	else
		return Util::toString(toStold(firstnum) * toStold(secondnum));
}

constr Calculate::divide(constr& firstnum, constr& secondnum)
{
	bool isff = Fraction::isFraction(firstnum); // IS First Fraction
	bool issf = Fraction::isFraction(secondnum); //IS Second Fraction
	
	if(isff || issf)
	{
		return Fraction(Fraction(firstnum) / Fraction(secondnum)).toString();
	}
	else
	{
		return Util::toString(toStold(firstnum) / toStold(secondnum));
	}
}

constr Calculate::power(constr& firstnum, constr& secondnum)
{
	bool isff = Fraction::isFraction(firstnum); // IS First Fraction
	bool issf = Fraction::isFraction(secondnum); //IS Second Fraction
	
	if(isff || issf)
	{
		return Fraction(Fraction(firstnum) ^ Fraction(secondnum)).toString();
	}
	else
		return Util::toString(pow(toStold(firstnum), toStold(secondnum)));
}

clongd Calculate::toStold(constr& num)
{
	try
	{
		return std::stold(num);
	}
	catch(...)
	{
		throw Util::Error::getMessage(Util::Error::ErrorName::InvalidExpression);
	}
}

} /* namespace Math */
