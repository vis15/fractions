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
typedef long long longl;
typedef const longl clongl;
typedef long double longd;
typedef const longd clongd;
typedef const bool cbool;

struct Message;
typedef std::vector<Message> messages_t;

constexpr cint kReturnError = -1;

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

enum class Verbosity : uint
{
	kNone, kError, kInfo, kDebug
};

namespace Info
{

inline constr kProgName() { return "Fractions"; }

} //namespace Info

namespace ColorsHTML
{

inline constr kPurple() { return "#703D8F"; }
inline constr kDPurple() { return "#4F0B78"; }
inline constr kBlue() { return "#0012FF"; }

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


constexpr char kSymbol[] = "+-*/^()";

inline constr kStack_Init() { return "$"; }
inline constr kOpenParenthesisR() { return "("; }
inline constr kCloseParenthesisR() { return ")"; }
inline constr kOpenParenthesisB() { return "["; }
inline constr kCloseParenthesisB() { return "]"; }
inline constr kOpenParenthesisC() { return "{"; }
inline constr kCloseParenthesisC() { return "}"; }
inline constr kMuliply() { return "*"; }
inline constr kDivide() { return "/"; }
inline constr kAdd() { return "+"; }
inline constr kSubtract() { return "-"; }
inline constr kPower() { return "^"; }
inline constr kDot() { return "."; }
inline constr kSqrtRoot() { return "√"; }

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

enum class FunctionDebug
{
	kParRep, kAddMul, kSubToAdd, kTokenize, kRPN, kCalc
};

inline namespace DefaultConfigVars
{

constexpr cint kwinposx = -1;
constexpr cint kwinposy = -1;
constexpr cint kwinsizel = 500;
constexpr cint kwinsizew = 500;
constexpr cbool kparrep = false;
constexpr cbool kparrep_p = false;
constexpr cbool kaddmul = false;
constexpr cbool kaddmul_p = false;
constexpr cbool ksubtoadd = false;
constexpr cbool ksubtoadd_p = false;
constexpr cbool ktokenize = false;
constexpr cbool ktokenize_p = false;
constexpr cbool krpn = false;
constexpr cbool krpn_p = false;
constexpr cbool kcalc = false;
constexpr cbool kcalc_p = false;
constexpr cbool kfractions = true;
constexpr cbool klog = true;
constexpr cbool kcolor = true;

} // namespace DefaultConfigVars

struct ClassVars
{
	bool debug = false;
	bool debugwin = false;
	str exp;
	bool fractions = true;
};

struct DebugWindowSettings
{
	bool parrep = kparrep;
	bool parrep_p = kparrep_p;
	bool addmul = kaddmul;
	bool addmul_p = kaddmul_p;
	bool subtoadd = ksubtoadd;
	bool subtoadd_p = ksubtoadd_p;
	bool tokenize = ktokenize;
	bool tokenize_p = ktokenize_p;
	bool rpn = krpn;
	bool rpn_p = krpn_p;
	bool calc = kcalc;
	bool calc_p = kcalc_p;
};

struct PrefsWindowSettings
{
	bool fractions = kfractions;
	bool log = klog;
	bool color = kcolor;
};

struct ConfigSettings
{
	int winposx = kwinposx;
	int winposy = kwinposy;
	int winsizel = kwinsizel;
	int winsizew = kwinsizew;
	DebugWindowSettings dwsettings;
	PrefsWindowSettings pwsettings;
};

struct Message
{
	Verbosity verbosity;
	str message;
};

} // namespace Math

#endif /* VARS_H_ */
