/*
 * vars.cpp
 *
 *  Created on: Nov 20, 2013
 *      Author: dis
 */
#include "vars.h"

namespace Math
{

namespace ColorsHTML
{

constr kPurple() { return "#703D8F"; }
constr kDPurple() { return "#4F0B78"; }

} //namespace ColorsHTML
namespace Parser
{

constr kStack_Init() { return "$"; }
constr kOpenParenthesisR() { return "("; }
constr kCloseParenthesisR() { return ")"; }
constr kOpenParenthesisB() { return "["; }
constr kCloseParenthesisB() { return "]"; }
constr kOpenParenthesisC() { return "{"; }
constr kCloseParenthesisC() { return "}"; }
constr kMuliply() { return "*"; }
constr kDivide() { return "/"; }
constr kAdd() { return "+"; }
constr kSubtract() { return "-"; }
constr kPower() { return "^"; }

} //namespace Parser
}//namespace Math
