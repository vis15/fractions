/*
 * util.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: dis
 */

#include <iostream>

#include "util.h"
#include "mainwindow.h"

namespace Math
{

namespace Util
{
///////////////////////////////////////////////////////////////////////////////////////
//Class Say
///////////////////////////////////////////////////////////////////////////////////////

Say::Say(const bool debug) : debug_(debug) {}

void Say::display(constr& message, const MessageState& msg_state)
{
	//mw_.updateStatus(message, msg_state);
	
	MessageState new_state = msg_state;
	if(debug_)
		new_state = MessageState::kNone;
	
	termDisplay(message, new_state);
}

void Say::termDisplay(constr& message, const MessageState& msg_state)
{
	switch(msg_state)
	{
	case MessageState::kInfo:
	case MessageState::kNone:
		std::cout << message << std::endl;
		break;
	case MessageState::kWarning:
	case MessageState::kError:
		std::cerr << termColor(TerminalColors::kRed) << message << termColor(TerminalColors::kNone) << std::endl; //switch term color back
		break;
	}
}

constr Say::termColor(const TerminalColors& foreground, const TerminalColors& background, const bool bold)
{
	constr esc_color = "\033[";
	constr esc_end = "m";
	constr color_none = "0";
	constr color_split = ";";
	constexpr cint f_color_start = 29;
	constexpr cint b_color_start = 39;
	constr esc_bold = esc_color + "1" + esc_end;
	
	str output = esc_color; //begin color
	
	if(foreground == TerminalColors::kNone && background == TerminalColors::kNone)
		output += color_none;
	
	if(foreground != TerminalColors::kNone)
	{
		output += Util::toString(f_color_start + TermColorsInt(foreground).color_num);
		if(background != TerminalColors::kNone) 
			output += color_split;
	}
	
	if(background != TerminalColors::kNone)
		output += Util::toString(b_color_start + TermColorsInt(background).color_num);
	
	output += esc_end;
	
	if(bold == true)
		output += esc_bold;
	
	return output;
}

///////////////////////////////////////////////////////////////////////////////////////
//Class Error
//////////////////////////////////////////////////////////////////////////////////////
Error::Error(bool debug): debug_(debug)
{
	error_name_ = Error::ErrorName::None;

}

Error::~Error()
{
	// 
}

constr Error::show(const ErrorName& error_name, constr opt_text, constr function)
{
	error_name_ = error_name;
	str text = "";
	
	switch(error_name)
	{
	case ErrorName::InvalidExpression:
		text = "Invalid Expression";
		break;
	case ErrorName::Unknown:
		text = "Unknown Error";
		break;
	case ErrorName::None:
		break;
	}
	
	str err_msg = "Error: " + text;
	
	if(opt_text != "")
		err_msg += " " + opt_text;
	
	if(function != "")
		err_msg += " in function: " + function;
	
	return err_msg;
}

int Error::getErrorValue()
{
	UerrorName uerr(error_name_);
	return uerr.value;
}

void Error::showThrow(const ErrorName& error_name, constr opt_text, constr function)
{
	throw show(error_name, opt_text, function);
}

} //namespace Util
} //namespace Math
