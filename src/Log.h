/*
 * Log.h
 *
 *  Created on: Mar 29, 2014
 *      Author: dis
 */

#ifndef LOG_H_
#define LOG_H_

#include "vars.h"

namespace Math
{

class Log
{
public:
	Log();
	virtual ~Log();
	void append(constr& text);
	bool check();
	void init(constr& working_dir, constr& logname, cbool enabled);
	
	str logfile_;
	bool enabled_;
};

} /* namespace Math */

#endif /* LOG_H_ */
