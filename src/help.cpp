/*
 * help.cpp
 *
 *  Created on: Dec 10, 2013
 *      Author: dis
 */

#include "help.h"

namespace Math
{

constr helpText()
{
	constr help_text = 
			R"helptext(
Command Help
--debug    Enables debug, no terminal color
-v         Verbose Info
-vv        Verbose Debug
-nv        Verbose None
)helptext";
	
	return help_text;
}

} //namespace Math
