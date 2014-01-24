/*
 * parser.cpp
 *
 *  Created on: Nov 2, 2013
 *      Author: dis
 */

//#include <stdlib.h>

#include <iostream>
#include <cctype>
#include <sstream>
#include <algorithm>

#include "parser.h"
#include "util.h"

//Todo: Threads! progress bar?

namespace Math
{

namespace Parser
{

Parser::Parser(ClassVars classvars)
{
	classvars_ = classvars;
}

Parser::~Parser()
{
	
}

cvstr Parser::parse()
{
	//todo: more cout
	//todo: another way to implement errors is by returning a struct with error or throw
	
	str output = "";
	output = parenthesisReplace(classvars_.exp);
	
	if(classvars_.debugwin == true)
		signal_debugoutput.emit(output, FunctionDebug::kParRep, false);
	
	output = addMultiply(output);
	
	if(classvars_.debugwin == true)
		signal_debugoutput.emit(output, FunctionDebug::kAddMul, false);
	
	output = subToAdd(output);
	
	if(classvars_.debugwin == true)
		signal_debugoutput.emit(output, FunctionDebug::kSubToAdd, false);
	
	vstr voutput;
	
	if(classvars_.fractions)
		voutput = fractionTokenize(output);
	else
		voutput = tokenize(output);
	
	if(classvars_.debugwin == true)
		signal_debugoutput.emit(Util::vecToString(voutput), FunctionDebug::kTokenize, false);
	
	voutput = toRPN(voutput);
	
	return voutput;
	//return vStrToStr(voutput);
}

cvstr Parser::fractionTokenize(constr& exp)
{
	vstr output;
	
	str num = "";
	str out = "";
	bool first_divide = true;
	str functionoutput = "";
	
	for(uint i=0; i<exp.size(); i++)
	{
		constr chr = Util::toString(exp.at(i));
		str nchr = "";
		if(i+1 < exp.size())
			nchr = Util::toString(exp.at(i+1));
		str lchr = "";
		if(i > 0)
			lchr = Util::toString(exp.at(i-1));
		
		if(isNumeric(chr))
		{
			num += chr;
		}
		else if(chr == kDivide() && first_divide && !isOpenParenthesis(nchr) && !isCloseParenthesis(lchr))
		{
				out = num + chr;
				first_divide = false;
				num = "";
		}
		else if(chr == kSubtract() && (isSymbol(lchr) || isParenthesis(lchr) || lchr == ""))
		{
			num += chr;
		}
		else if(chr == kAdd() && (isSymbol(lchr) || isParenthesis(lchr) || lchr == ""))
		{
			continue; //do not add + it's a duplicate
		}
		else
		{
			if(out != "")
			{
				out += num;
				output.push_back(out);
				out = "";
			}
			else if(num != "")
			{
				output.push_back(num);
			}
			
			output.push_back(chr);
			num = "";
			first_divide = true;
		}
		
		if(classvars_.debugwin)
		{
			functionoutput += "Expression: " + exp.substr(i+1,exp.length()) + "\n";
			functionoutput += "Num: " + num + "\n";
			functionoutput += "Fraction: " + out + "\n";
			functionoutput += "Output: " + Util::vStrToStr(output) + "\n\n";
		}
	}
	
	//push rest of out and num to output
	if(out != "")
	{
		out += num;
		output.push_back(out);
	}
	else if(num != "") //push last number from the queue
		output.push_back(num);
	
	if(classvars_.debugwin)
		signal_debugoutput.emit(functionoutput, FunctionDebug::kTokenize, true);
	
	return output;
}

cvstr Parser::tokenize(constr& exp)
{
	vstr output;
		
	str num = "";
	str functionoutput = "";
	
	for(uint i=0; i<exp.size(); i++)
	{
		str chr = Util::toString(exp.at(i)); 
		str lchr = "";
		if(i > 0)
			lchr = Util::toString(exp.at(i-1));
		
		if(isNumeric(chr))
		{
			num += chr;
		}
		else if(chr == kSubtract() && (isSymbol(lchr) || isParenthesis(lchr) || lchr == ""))
		{	
			num += chr;
		}
		else if(chr == kAdd() && (isSymbol(lchr) || isParenthesis(lchr) || lchr == ""))
		{
			continue; //do not add + it's a duplicate
		}
		else
		{
			if(num != "")
				output.push_back(num);
			
			output.push_back(chr);
			num = "";
		}
		
		if(classvars_.debugwin)
		{
			functionoutput += "Expression: " + exp.substr(i+1,exp.length()) + "\n";
			functionoutput += "Num: " + num + "\n";
			functionoutput += "Output: " + Util::vStrToStr(output) + "\n\n";
		}
	}
	
	 if(num != "") //push last number from the queue
		output.push_back(num);
	 
	if(classvars_.debugwin)
		signal_debugoutput.emit(functionoutput, FunctionDebug::kTokenize, true);
	
	return output;
}

constr Parser::addMultiply(constr& exp)
{
	str atmp = exp;
	int k = 0; 
	str functionoutput = "";

	for (uint i = 1; i < exp.length(); i++) //ignore first char
	{
		constr chr = Util::toString(exp.at(i));

		constr last_char = Util::toString(exp.at(i - 1));

		if (isOpenParenthesis(chr) && (!isSymbol(last_char) && !isOpenParenthesis(last_char))) //i>0 = not first "("
		{
			atmp.insert(i + k, kMuliply());
			k++; //when adding '*' length of atmp increases by 1
		}
		
		if(classvars_.debugwin)
		{
			functionoutput += "Expression: " + exp.substr(i, exp.length()) + "\n";
			functionoutput += "New Expression: " + atmp.substr(0, i+k) + "\n\n";
		}
	}
	
	if(classvars_.debugwin)
		signal_debugoutput.emit(functionoutput, FunctionDebug::kAddMul, true);
	
	return atmp;
}

/*
 * Function: subToAdd(str& exp)
 * 
 * This function replaces multiple subtraction to addition
 * example: 1--2 = 1+2
 */
constr Parser::subToAdd(str& exp)
{
	str output = "";
	str functionoutput = "";
	
	uint i = 0;
	while(exp.length() != 0)
	{
		str chr = Util::toString(exp.at(0));
		str nchr = ""; //next char
		if(exp.length() > 1)
			nchr = Util::toString(exp.at(1));
		
		if(chr == kSubtract() && nchr == kSubtract())
		{
			output += kAdd();
			exp.erase(1, 1);
		}
		else
		{
			output += chr;
		}
		
		exp.erase(0, 1);
		
		if(classvars_.debugwin)
		{
			functionoutput += "Expression: " + exp + "\n";
			functionoutput += "Output: " + output + "\n\n";
		}
		
		i++;
	}
	
	if(classvars_.debugwin)
		signal_debugoutput.emit(functionoutput, FunctionDebug::kSubToAdd, true);
	
	return output;
}

cvstr Parser::toRPN(cvstr& exp)
{
	//Todo: Make function that checks the validity of equation
	vstr output;
	std::stack<str> stk;
	constr stack_init = kStack_Init();
	str outputstr = "";
	
	auto func_update = [&output, &stk]() mutable { output.push_back(stk.top()); stk.pop(); }; //lambda
	
	stk.push(stack_init); //begin stack character

	//shunting-yard algorithm to turn reg notation into RPN
		
	for(uint i=0; i<exp.size(); i++)
	{
		constr token = exp.at(i);
		
		if(isSymbol(token))
		{
			const Associative associative = getAssociative(token);
			
			while(stk.top() != stack_init && ((isLeftAssociative(associative) && isPrecedenceEqual(token, stk.top())) || isPrecedenceLess(token, stk.top())))
			{
				func_update();
			}
			
			stk.push(token);
		}
		else if(isOpenParenthesis(token))
		{
			stk.push(token);
		}
		else if(isCloseParenthesis(token))
		{
			while(stk.top() != kOpenParenthesisR())
			{
				func_update();
			}
			
			stk.pop(); //pop open parenthesis
		}
		else //if(isNumeric(token)) //faster
		{
			output.push_back(token);
		}
		
		if(classvars_.debugwin == true)
		{
			outputstr += "Expression: " + Util::vStrToStr(exp, i+1) + "\n";
			outputstr += "Stack: " + Util::stackStrToStr(stk) + "\n";
			outputstr += "Queue: " + Util::vStrToStr(output) + "\n";
			outputstr += "\n";
		}
	}
	
	while(stk.size() > 1)
	{
		output.push_back(stk.top());
		stk.pop();
		if(stk.top() == stack_init)
			break;
	}
	
	if(classvars_.debugwin == true)
		signal_debugoutput.emit(outputstr, FunctionDebug::kRPN, true);
	
	return output;
}

void Parser::checkVars(constr& str, constr& next_str)
{
	using Util::Error;
	
	Error err();
	if(str == "")
	{
		//err.showThrow(Error::ErrorName::InvalidExpression);
	}
}

void Parser::checkVars(const Fraction& fraction)
{
	using Util::Error;
	
//	Error err(mw_);
//	if(fraction.denominator_ == 0 || fraction.numerator_ == 0)
//	{
//		err.showThrow(Error::ErrorName::InvalidExpression);
//	}
}

void Parser::test()
{
	checkVars("");
}

void Parser::setExp(constr& expression)
{
	classvars_.exp = expression;
}

constr Parser::strReplace(str& str, char old, char snew)
{
	std::replace(str.begin(), str.end(), old, snew);
	
	return str;
}

constr Parser::parenthesisReplace(str& exp)
{
	str functionoutput = "";
	
	exp = strReplace(exp, '[', '(');
	functionoutput += "Bracket Start: " + exp + "\n";
	exp = strReplace(exp, ']', ')');
	functionoutput += "Bracket End: " + exp + "\n";
	exp = strReplace(exp, '{', '(');
	functionoutput += "Brace Start: " + exp + "\n";
	exp = strReplace(exp, '}', ')');
	functionoutput += "Brace End: " + exp + "\n";
	
	if(classvars_.debugwin)
		signal_debugoutput.emit(functionoutput, FunctionDebug::kParRep, true);
	
	return exp;
}

} //namespace Parser
} //namespace Math
