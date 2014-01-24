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
	debug_ = false;
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
	else if(svarname == kParRep)
	{
		if(varnum == kReturnError)
			varnum = kparrep;
		
		config_.dwsettings.parrep = varnum;
	}
	else if(svarname == kParRep_p)
	{
		if(varnum == kReturnError)
			varnum = kparrep_p;
		
		config_.dwsettings.parrep_p = varnum;
	}
	else if(svarname == kAddMul)
	{
		if(varnum == kReturnError)
			varnum = kaddmul;
		
		config_.dwsettings.addmul = varnum;
	}
	else if(svarname == kAddMul_p)
	{
		if(varnum == kReturnError)
			varnum = kaddmul_p;
		
		config_.dwsettings.addmul_p = varnum;
	}
	else if(svarname == kSubToAdd)
	{
		if(varnum == kReturnError)
			varnum = ksubtoadd;
		
		config_.dwsettings.subtoadd = varnum;
	}
	else if(svarname == kSubToAdd_p)
	{
		if(varnum == kReturnError)
			varnum = ksubtoadd_p;
		
		config_.dwsettings.subtoadd_p = varnum;
	}
	else if(svarname == kTokenize)
	{
		if(varnum == kReturnError)
			varnum = ktokenize;
		
		config_.dwsettings.tokenize = varnum;
	}
	else if(svarname == kTokenize_p)
	{
		if(varnum == kReturnError)
			varnum = ktokenize_p;
		
		config_.dwsettings.tokenize_p = varnum;
	}
	else if(svarname == kRPN)
	{
		if(varnum == kReturnError)
			varnum = krpn;
		
		config_.dwsettings.rpn = varnum;
	}
	else if(svarname == kRPN_p)
	{
		if(varnum == kReturnError)
			varnum = krpn_p;
		
		config_.dwsettings.rpn_p = varnum;
	}
	else if(svarname == kCalc)
	{
		if(varnum == kReturnError)
			varnum = kcalc;
		
		config_.dwsettings.calc = varnum;
	}
	else if(svarname == kCalc_p)
	{
		if(varnum == kReturnError)
			varnum = kcalc_p;
		
		config_.dwsettings.calc_p = varnum;
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
		str value;
	};

	typedef const ConfigVars ccv;

	using namespace ConfigVar;
	
	ccv window_sizel = {kWindowSizeL, Util::toString(config.winsizel) };
	ccv window_sizew = {kWindowSizeW, Util::toString(config.winsizew) };
	ccv window_positionx = {kWindowPositionX, Util::toString(config.winposx) };
	ccv window_positiony = {kWindowPositionY, Util::toString(config.winposy) };
	ccv parrep = {kParRep, Util::toString(config.dwsettings.parrep) };
	ccv parrep_p = {kParRep_p, Util::toString(config.dwsettings.parrep_p) };
	ccv addmul = {kAddMul, Util::toString(config.dwsettings.addmul) };
	ccv addmul_p = {kAddMul_p, Util::toString(config.dwsettings.addmul_p) };
	ccv subtoadd = {kSubToAdd, Util::toString(config.dwsettings.subtoadd) };
	ccv subtoadd_p = {kSubToAdd_p, Util::toString(config.dwsettings.subtoadd_p) };
	ccv tokenize = {kTokenize, Util::toString(config.dwsettings.tokenize) };
	ccv tokenize_p = {kTokenize_p, Util::toString(config.dwsettings.tokenize_p) };
	ccv rpn = {kRPN, Util::toString(config.dwsettings.rpn) };
	ccv rpn_p = {kRPN_p, Util::toString(config.dwsettings.rpn_p) };
	ccv calc = {kCalc, Util::toString(config.dwsettings.calc) };
	ccv calc_p = {kCalc_p, Util::toString(config.dwsettings.calc_p) };

	typedef std::vector<ConfigVars> configvars;

	const configvars kConfig = {window_sizel, window_sizew, window_positionx, window_positiony, 
		parrep, parrep_p, addmul, addmul_p, subtoadd, subtoadd_p, tokenize, tokenize_p, rpn, rpn_p, calc, calc_p};
	
	str configstr = "";
	
	//build config output
	for(uint i=0; i<kConfig.size(); i++)
	{
		configstr += kConfig.at(i).var + ": " + kConfig.at(i).value + "\n";
	}
	
	Util::writeFileToDisk(kConfigFile_, configstr);
}

} // namespace Math
