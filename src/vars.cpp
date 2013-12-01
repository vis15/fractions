/*
 * vars.cpp
 *
 *  Created on: Nov 20, 2013
 *      Author: dis
 */
#include "vars.h"

namespace Math
{

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
