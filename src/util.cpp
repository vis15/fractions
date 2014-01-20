/*
 * util.cpp
 *
 *  Created on: Nov 3, 2013
 *      Author: dis
 */

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "util.h"

namespace Math
{

bool isOpenParenthesis(const str& token)
{
	if(token == kOpenParenthesisR() || token == kOpenParenthesisB() || token == kOpenParenthesisC())
		return true;
	
	return false;
}

bool isCloseParenthesis(const str& token)
{
	if(token == kCloseParenthesisR() || token == kCloseParenthesisB() || token == kCloseParenthesisC())
		return true;
	
	return false;
}


bool isParenthesis(const str& token)
{
	if(isOpenParenthesis(token) || isCloseParenthesis(token))
		return true;
	
	return false;
}

bool isMulOrDivide(const str& token)
{
	if(token == kMuliply() || token == kDivide())
		return true;
	
	return false;
}

bool isAddOrSub(const str& token)
{
	if(token == kAdd() || token == kSubtract())
		return true;
	
	return false;
}

bool isPower(const str& token)
{
	if(token == kPower())
		return true;
	
	return false;
}

bool isSymbol(const str& token)
{
	if(isAddOrSub(token) || isMulOrDivide(token) || isPower(token))
		return true;
	
	return false;
}

bool isNumeric(const str& token)
{
	if(token == "")
		return false;
	
	for(uint i=0; i<token.length(); i++)
	{
		const char num = token.at(i);
		if(i == 0 && Util::toString(num) == kSubtract() && token.size() > 1) //token.size > 1 because "-" is not a number
			continue;
		if(isdigit(num) == false && Util::toString(num) != kDot())
			return false;
	}
	
	return true;
}

Precedence precedence(const str& token)
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

Associative getAssociative(const str& token)
{
	if(isPower(token))
		return Associative::kRight;
	
	return Associative::kLeft;
}

bool isLeftAssociative(Associative associative)
{
	if(associative == Associative::kLeft)
		return true;
	
	return false;
}

bool isPrecedenceLess(const str& token, const str& otoken)
{
	if(precedence(token) < precedence(otoken))
		return true;
	
	return false;
}

bool isPrecedenceEqual(const str& token, const str& otoken)
{
	if(precedence(token) == precedence(otoken))
		return true;
		
	return false;
}

namespace Util
{

struct matchPathSeparator
{
    bool operator()( char ch ) const
    {
        return ch == '/';
    }
};

constr getFilenameFromPath(constr& file)
{
	return std::string(std::find_if(file.rbegin(), file.rend(), matchPathSeparator()).base(), file.end());
}

constr getDirFromPath(constr& filepath)
{
	return filepath.substr(0, filepath.find_last_of("/"));
}

cint saveFile(constr& file, constr& text)
{
	std::ofstream outfile(file);
	
	if(! outfile.is_open()) 
		return 1;
	
	outfile << text;
	
	outfile.close();
	
	return 0;
}

void debugSay(constr& msg)
{
	std::cout << msg << std::endl;
}

bool checkIfFileExist(constr& file)
{
	std::ifstream cfile(file);
	
	return cfile;
}

void writeFileToDisk(constr& filepath, constr& data)
{	
	std::ofstream ofile(filepath);
	
	if(! ofile.is_open())
	{
		Error err(false);
		err.sayError(Error::ErrorName::NoFileOpen, filepath, __func__);
		return;
	}
	
	ofile << data;
	
	ofile.close();
}

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
	constr start_str = Info::kProgName() + ": ";
	switch(msg_state)
	{
	case MessageState::kInfo:
	case MessageState::kNone:
		std::cout << start_str << message << std::endl;
		break;
	case MessageState::kWarning:
	case MessageState::kError:
		std::cerr << termColor(TerminalColors::kRed) << start_str << message << termColor(TerminalColors::kNone) << std::endl; //switch term color back
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

constr Error::getMessage(const ErrorName& error_name, constr opt_text, constr function)
{
	//todo Errors should be in a text file for ease of use and in8t
	str text = "";
	
	switch(error_name)
	{
	case ErrorName::InvalidExpression:
		text = "Invalid Expression";
		break;
	case ErrorName::NoFileOpen:
		text = "File: " + opt_text + " could not be opened";
		break;
	case ErrorName::ReadingConfig:
		text = "Reading configuration file";
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
	error_name_ = error_name;
	throw getMessage(error_name, opt_text, function);
}

void Error::sayError(const ErrorName& error_name, constr opt_text, constr function)
{
	Say say(debug_);
	say.display(Error::getMessage(error_name, opt_text, function), MessageState::kError);
}

} //namespace Util
} //namespace Math
