/*
 * vars.h
 *
 *  Created on: Nov 2, 2013
 *      Author: dis
 */

#ifndef VARS_H_
#define VARS_H_

#include <string>
#include <array>
#include <vector>
#include <map>

namespace Math
{

typedef const std::string constr;
typedef std::string str;
typedef std::vector<str> vstr;
typedef const vstr cvstr;
typedef const double condbl;
typedef const long clong;
typedef const int cint;
typedef const uint cuint;
typedef std::vector<uint> vuint;

template <typename Type, typename Output> //T=type, O=output
struct Sout //for returning multiple outputs
{
	Type out1;
	Output out2;
};

enum class Operator
{
	add, multiply, divide
};

struct MessageMarkup
{
	MessageMarkup() : color(""), bg_color(""), size(""), bold(false), underline(false), italic(false){}
	str color;
	str bg_color;
	str size;
	bool bold;
	bool underline;
	bool italic;
};

enum class MessageState
{
	kNone, kInfo, kWarning, kError
};

enum class Verbosity
{
	kNone, kError, kInfo, kDebug
};

namespace ColorsHTML
{

constr kPurple();
constr kDPurple();

} //namespace ColorsHTML

namespace Util
{

enum class TerminalColors : uint
{
	kNone = 0,
	kBlack, kRed, kGreen,
	kYellow, kBlue, kPurple,
	kCyan, kWhite
};

union TermColorsInt
{
	TermColorsInt(TerminalColors color) : term_color(color) {}
	TermColorsInt() {}
	TerminalColors term_color;
	uint color_num;
};

} //namespace Util

namespace Parser
{

constexpr char kSymbol[] = "+-*/^()";

constr kStack_Init();
constr kOpenParenthesisR();
constr kCloseParenthesisR();
constr kOpenParenthesisB();
constr kCloseParenthesisB();
constr kOpenParenthesisC();
constr kCloseParenthesisC();
constr kMuliply();
constr kDivide();
constr kAdd();
constr kSubtract();
constr kPower();

enum class Associative
{
	kLeft, kRight
};

enum class Precedence : uint
{
	kUnknown = 0,
	kAdd_Sub = 1,
	kMul_Div = 2,
	kPower = 3
};

} //namespace Parser

} //namespace Math

#endif /* VARS_H_ */
