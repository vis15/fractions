/*
 * parser.h
 *
 *  Created on: Nov 2, 2013
 *      Author: dis
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stack>

#include <sigc++-2.0/sigc++/sigc++.h>

#include "fraction.h"
#include "vars.h"

namespace Math
{

namespace Parser
{

class Parser
{
public:
	Parser(constr& expression, bool fractions_enabled, bool debug = false);
	virtual ~Parser();
	cvstr parse();
	void setExp(constr& expression);
	//bool isValid();
	void test();
	constr vStrToStr(const vstr& vec_str, int spos = 0) const; //spos = start position
	constr stackStrToStr(std::stack<str>& stack_str) const;
	cvstr toRPN(cvstr& exp);
	constr addMultiply(constr& exp);
	cvstr fractionTokenize(constr& exp);
	constr strReplace(str& str, char old, char snew);
	constr parenthesisReplace(str& exp);
	constr subToAdd(str& exp);
	cvstr tokenize(constr& exp);
	
private:
	void checkVars(constr& str, constr& next_str = "");
	void checkVars(const Fraction& fraction);
	
public:
	str exp_;
	sigc::signal<void, constr&, const MessageState&> signal_say;
	sigc::signal<void, constr&, FunctionDebug> signal_debugoutput;
	
private:
	bool fractions_enabled_;
	const bool debug_;
};

} //namespace Parser
} //namespace Math

#endif /* PARSER_H_ */
