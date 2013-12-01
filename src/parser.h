/*
 * parser.h
 *
 *  Created on: Nov 2, 2013
 *      Author: dis
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stack>

#include<sigc++-2.0/sigc++/sigc++.h>

#include "fraction.h"
#include "vars.h"

namespace Math
{

namespace Parser
{

class Parser
{
public:
	Parser(constr& expression, bool debug = false);
	virtual ~Parser();
	constr parse();
	void setExp(constr& expression);
	//bool isValid();
	void test();
	
private:
	void checkVars(constr& str, constr& next_str = "");
	void checkVars(const Fraction& fraction);
	bool isOpenParenthesis(const str& token);
	bool isCloseParenthesis(const str& token);
	bool isParenthesis(const str& token);
	bool isMulOrDivide(const str& token);
	bool isAddOrSub(const str& token);
	bool isPower(const str& token);
	bool isSymbol(const str& token);
	bool isNumeric(const str& token);
	Precedence precedence(const str& token);
	Associative getAssociative(const str& token);
	bool isLeftAssociative(Associative associative);
	bool isPrecedenceLess(const str& token, const str& otoken);
	bool isPrecedenceEqual(const str& token, const str& otoken);
	constr vStrToStr(vstr vec_str, int spos = 0); //spos = start position
	constr stackStrToStr(std::stack<str> stack_str);
	cvstr toRPN(cvstr exp);
	constr addMultiply(constr exp);
	cvstr tokenize(constr exp);
	constr strReplace(str str, char old, char snew);
	constr parenthesisReplace(str exp);
	
public:
	str exp_;
	sigc::signal<void, constr&, const MessageState&> signal_say;
	
private:
	const bool debug_;
};

} //namespace Parser
} //namespace Math

#endif /* PARSER_H_ */
