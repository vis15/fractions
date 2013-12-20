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

Parser::Parser(constr& expression, bool debug) : exp_(expression), debug_(debug)
{
	
}

Parser::~Parser()
{
	
}

constr Parser::parse()
{
	//todo: more cout
	//todo: another way to implement errors is by returning a struct with error or throw
	
	str output = "";
	output = parenthesisReplace(exp_);
	output = addMultiply(output);
	
	vstr voutput;
	voutput = tokenize(output);
	voutput = toRPN(voutput);
	
	return vStrToStr(voutput);
}

cvstr Parser::tokenize(constr exp)
{
	vstr output;
	
	str num = "";
	str out = "";
	bool first_divide = true;
	
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
	}
	
	//push rest of out and num to output
	if(out != "")
	{
		out += num;
		output.push_back(out);
	}
	else if(num != "") //push last number from the queue
		output.push_back(num);
	
	return output;
}

constr Parser::addMultiply(constr exp)
{
	str atmp = exp;
	int k = 0; 

	for (uint i = 1; i < exp.length(); i++) //ignore first char
	{
		constr chr = Util::toString(exp.at(i));

		constr last_char = Util::toString(exp.at(i - 1));

		if (isOpenParenthesis(chr) && isSymbol(last_char) == false) //i>0 = not first "("
		{
			atmp.insert(i + k, "*");
			k++; //when adding '*' length of atmp increases by 1
		}
	}
	
	return atmp;
}

cvstr Parser::toRPN(cvstr exp)
{
	//Todo: Make function that checks the validity of equation
	//Todo: const
	vstr output;
	std::stack<str> stk;
	constr stack_init = kStack_Init();
	bool debug = false;
	
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
		
		if(debug)
		{
//			cout << "String: " << vStrToStr(b, i+1) << endl;
//			cout << "Queue: " << vStrToStr(bb) << endl;
//			cout << "Stack: " << stackStrToStr(stk) << endl;
//			cout << endl;
		}
	}
	
	while(stk.size() > 1)
	{
		output.push_back(stk.top());
		stk.pop();
		if(stk.top() == stack_init)
			break;
	}
	
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
	exp_=expression;
}

bool Parser::isOpenParenthesis(const str& token)
{
	if(token == kOpenParenthesisR() || token == kOpenParenthesisB() || token == kOpenParenthesisC())
		return true;
	
	return false;
}

bool Parser::isCloseParenthesis(const str& token)
{
	if(token == kCloseParenthesisR() || token == kCloseParenthesisB() || token == kCloseParenthesisC())
		return true;
	
	return false;
}


bool Parser::isParenthesis(const str& token)
{
	if(isOpenParenthesis(token) || isCloseParenthesis(token))
		return true;
	
	return false;
}

bool Parser::isMulOrDivide(const str& token)
{
	if(token == kMuliply() || token == kDivide())
		return true;
	
	return false;
}

bool Parser::isAddOrSub(const str& token)
{
	if(token == kAdd() || token == kSubtract())
		return true;
	
	return false;
}

bool Parser::isPower(const str& token)
{
	if(token == kPower())
		return true;
	
	return false;
}

bool Parser::isSymbol(const str& token)
{
	if(isAddOrSub(token) || isMulOrDivide(token) || isPower(token))
		return true;
	
	return false;
}

bool Parser::isNumeric(const str& token)
{
	for(uint i=0; i<token.length(); i++)
	{
		const char num = token.at(i);
		if(isdigit(num) == false)
			return false;
	}
	
	return true;
}

Precedence Parser::precedence(const str& token)
{
	if(isAddOrSub(token))
		return Precedence::kAdd_Sub;
	else if(isMulOrDivide(token))
		return Precedence::kMul_Div;
	else if(isPower(token))
		return Precedence::kPower;
	else
		return Precedence::kUnknown;
}

Associative Parser::getAssociative(const str& token)
{
	if(isPower(token))
		return Associative::kRight;
	
	return Associative::kLeft;
}

bool Parser::isLeftAssociative(Associative associative)
{
	if(associative == Associative::kLeft)
		return true;
	
	return false;
}

bool Parser::isPrecedenceLess(const str& token, const str& otoken)
{
	if(precedence(token) < precedence(otoken))
		return true;
	
	return false;
}

bool Parser::isPrecedenceEqual(const str& token, const str& otoken)
{
	if(precedence(token) == precedence(otoken))
		return true;
		
	return false;
}

constr Parser::vStrToStr(vstr vec_str, int spos) //spos = start position
{
	str str = "";
	
	for(uint i=spos; i<vec_str.size(); i++)
	{
		str += " " + vec_str.at(i); //space to show where tokens start and end
	}
	
	return str;
}

constr Parser::stackStrToStr(std::stack<str> stack_str)
{
	str str = "";
	
	while(stack_str.size())
	{
		str += stack_str.top();
		stack_str.pop();
	}
	
	return str;
}

constr Parser::strReplace(str str, char old, char snew)
{
	std::replace(str.begin(), str.end(), old, snew);
	
	return str;
}

constr Parser::parenthesisReplace(str exp)
{
	exp = strReplace(exp, '[', '(');
	exp = strReplace(exp, ']', ')');
	exp = strReplace(exp, '{', '(');
	exp = strReplace(exp, '}', ')');
	
	return exp;
}

} //namespace Parser
} //namespace Math
