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
	Parser(ClassVars classvars);
	virtual ~Parser();
	cvstr parse();
	void setExp(constr& expression);
	//bool isValid();
	void test();
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
	sigc::signal<void, constr&, const MessageState&> signal_say;
	sigc::signal<void, constr&, FunctionDebug, bool> signal_debugoutput;
	
private:
	ClassVars classvars_;
};

} //namespace Parser
} //namespace Math

#endif /* PARSER_H_ */
