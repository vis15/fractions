/*
 * util.h
 *
 *  Created on: Nov 2, 2013
 *      Author: dis
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <sstream>

#include "vars.h"

namespace Math
{

namespace Util
{

template<typename T>
constr toString(T t)
{
	std::stringstream s;
	s << t;
	return s.str();
}

template<typename Type>
constr vecToString(std::vector<Type> data)
{
	str output = "";
	for(uint i=0; i<data.size(); i++)
	{
		output += Util::toString(i) + ": " + Util::toString(data.at(i)) + "\n";
	}
	
	return output;
}

constr getFilename(constr file);
cint saveFile(constr file, constr text);

///////////////////////////////////////////////////////////////////////////////////////
//Class Say
///////////////////////////////////////////////////////////////////////////////////////

class Say
{
public:
	Say(const bool debug);
	void display(constr& message, const MessageState& msg_state = MessageState::kNone);
	void termDisplay(constr& message, const MessageState& msg_state = MessageState::kNone);
	
private:
	constr termColor(const TerminalColors& foreground, const TerminalColors& background = TerminalColors::kNone, const bool bold = false);
	
private:
	const bool debug_;
};

///////////////////////////////////////////////////////////////////////////////////////
//Class Error
///////////////////////////////////////////////////////////////////////////////////////

class Error
{
public:
	enum class ErrorName : int
	{
		None = 0,
		InvalidExpression,
		NoFileOpen,
		Unknown = -1
	};
	
	union UerrorName
	{
		UerrorName(ErrorName err_type) : err_name(err_type) {}
		UerrorName(){}
		ErrorName err_name;
		int value;
	};

public:
	Error(bool debug = false);
	virtual ~Error();
	static constr getMessage(const ErrorName& error_name, constr opt_text = "", constr function = "");
	int getErrorValue();
	void showThrow(const ErrorName& error_name, constr opt_text = "", constr function = "");
	
public:
	ErrorName error_name_;
	
private:
	bool debug_ = false;
};

} //namespace Util
} //namespace Math


#endif /* UTIL_H_ */

