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

Calculate::Calculate(ClassVars classvars)
{
	classvars_ = classvars;
}

Calculate::~Calculate()
{
	// 
}

constr Calculate::calculate()
{
	vstr voutput;
	Parser::Parser p(classvars_);
	p.signal_debugoutput.connect(sigc::mem_fun(*this, &Calculate::debugOutput));
	
	try
	{
		voutput = p.parse();
	}
	catch(constr& err)
	{
		throw err;
	}
	
	if(classvars_.debugwin == true)
		signal_debugoutput.emit(Util::vecToString(voutput), FunctionDebug::kRPN, false);
	
	constr output = calcRPN(voutput);
	
	if(classvars_.debugwin == true)
		signal_debugoutput.emit(output, FunctionDebug::kCalc, false);
	
	return output;
}

void Calculate::debugOutput(constr& text, FunctionDebug function, bool procedure)
{
	signal_debugoutput.emit(text, function, procedure);
}

constr Calculate::calcRPN(cvstr& exp)
{
	std::stack<str> stk;
	constr stack_init = kStack_Init();
	str last = "";
	str last2 = "";
	str functionoutput = "";
	
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
		
		if(classvars_.debugwin)
		{
			functionoutput += "Expression: " + Util::vStrToStr(exp, i+1) + "\n";
			functionoutput += "Stack: " + Util::stackStrToStr(stk) + "\n\n";
		}
	}
	
	if(classvars_.debugwin)
		signal_debugoutput.emit(functionoutput, FunctionDebug::kCalc, true);
	
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
