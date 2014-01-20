/*
 * config.cpp
 *
 *  Created on: Jan 16, 2014
 *      Author: dis
 */

#include <regex>
#include <fstream>
#include <sstream>

#include "util.h"
#include "config.h"

namespace Math
{

constr Config::kFileExt_ = ".config";
constr Config::kFileName_ = Info::kProgName() + kFileExt_;

Config::Config()
{

}

Config::~Config()
{
	
}

void Config::init(constr& working_dir, const bool debug)
{
	debug_ = debug;
	
	kConfigFile_ = Util::getDirFromPath(working_dir) + "/" + kFileName_;
	
	if(! check())
	{
		createDefaultConfig();
		return;
	}
}

bool Config::check()
{
	return Util::checkIfFileExist(kConfigFile_);
}

void Config::createDefaultConfig()
{
	ConfigSettings config;
	updateConfig(config);
}

ConfigSettings Config::parseConfig()
{
	std::ifstream configfile(kConfigFile_);
	
	if(! configfile.is_open())
	{
		using Util::Error;
		Error err(false);
		err.sayError(Error::ErrorName::NoFileOpen, kConfigFile_, __func__);
		return config_;
	}
	
	str line = "";
	std::regex rx("(.*: )(.*)");
	std::smatch rxm;
	
	while(getline(configfile, line))
	{
		if(std::regex_match(line, rxm, rx))
		{
			parseVars(Util::toString(rxm[1]), Util::toString(rxm[2]));
		}
	}
	
	return config_;
}

void Config::parseVars(constr varname, constr var)
{
	using namespace ConfigVar;
	using namespace DefaultConfigVars;
	
	
	constr svarname = varname.substr(0, varname.size()-2); //strip ": "
	int varnum = toStoi(var);
	
	if(svarname == kWindowSizeL)
	{
		if(varnum == kReturnError)
			varnum = kwinsizel;
		
		config_.winsizel = varnum;	
	}
	else if(svarname == kWindowSizeW)
	{
		if(varnum == kReturnError)
			varnum = kwinsizew;
		
		config_.winsizew = varnum;	
	}
	else if(svarname == kWindowPositionX)
	{
		if(varnum == kReturnError)
			varnum = kwinposx;
		
		config_.winposx = varnum;
	}
	else if(svarname == kWindowPositionY)
	{
		if(varnum == kReturnError)
			varnum = kwinposy;
		
		config_.winposy = varnum;
	}
}

cint Config::toStoi(constr num)
{
	try
	{
		return std::stoi(num); //stoi doesn't care about spaces thankfully, unless there is a space in the middle
	}
	catch(...)
	{
		Util::Error err(debug_);
		err.sayError(Util::Error::ErrorName::ReadingConfig);
		return kReturnError;
	}
}

void Config::updateConfig(ConfigSettings config)
{
	struct ConfigVars
	{
		str var;
		str default_value;
	};

	typedef const ConfigVars ccv;

	using namespace ConfigVar;
	
	ccv window_sizel = {kWindowSizeL, Util::toString(config.winsizel) };
	ccv window_sizew = {kWindowSizeW, Util::toString(config.winsizew) };
	ccv window_positionx = {kWindowPositionX, Util::toString(config.winposx) };
	ccv window_positiony = {kWindowPositionY, Util::toString(config.winposy) };

	typedef std::vector<ConfigVars> configvars;

	const configvars kConfig = {window_sizel, window_sizew, window_positionx, window_positiony};
	
	str configstr = "";
	
	//build config output
	for(uint i=0; i<kConfig.size(); i++)
	{
		configstr += kConfig.at(i).var + ": " + kConfig.at(i).default_value + "\n";
	}
	
	Util::writeFileToDisk(kConfigFile_, configstr);
}

} // namespace Math
