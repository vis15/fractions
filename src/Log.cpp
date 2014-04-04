/*
 * Log.cpp
 *
 *  Created on: Mar 29, 2014
 *      Author: dis
 */

#include "Log.h"
#include "util.h"

namespace Math
{

Log::Log()
{
	enabled_ = DefaultConfigVars::klog;
}

Log::~Log()
{
	
}

void Log::append(constr& text)
{
	if(!enabled_)
		return;
	
	if(check())
		Util::appendFile(logfile_, text);
}

bool Log::check()
{
	if(Util::checkIfFileExist(logfile_))
	{
		return true;
	}
	else
	{
		if(Util::saveFile(logfile_, "") > 0)
		{
			using Util::Error;
			Error err(false);
			err.sayError(Error::ErrorName::NoFileOpen, logfile_, __func__);
			return false;
		}
		else
			return true;
	}
}

void Log::init(constr& working_dir, constr& logname, cbool enabled)
{
	enabled_ = enabled;
	logfile_ = working_dir + "/" + logname;
}

} /* namespace Math */
