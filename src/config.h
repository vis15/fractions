/*
 * config.h
 *
 *  Created on: Jan 16, 2014
 *      Author: dis
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "vars.h"

namespace Math
{

class Config
{
	Config& operator=(const Config) = delete; //disables copying
		
public:
	Config();
	virtual ~Config();
	bool check();
	void createDefaultConfig();
	ConfigSettings parseConfig();
	cint toStoi(constr num);
	void updateConfig(ConfigSettings config);
	void init(constr& working_dir, const bool debug, const Verbosity progverbose);
	
	static constr kFileName_;
	static constr kFileExt_;
	str kConfigFile_;
	ConfigSettings config_;
	
private:
	void parseVars(constr varname, constr var);
	
	bool debug_;
	Verbosity progverbose_;
};

namespace ConfigVar
{

constr kWindowSizeL = "WindowSizeL";
constr kWindowSizeW = "WindowSizeW";
constr kWindowPositionX = "WindowPositionX";
constr kWindowPositionY = "WindowPositionY";
constr kParRep = "Parrep";
constr kParRep_p = "ParRep_p";
constr kAddMul = "AddMul";
constr kAddMul_p = "AddMul_p";
constr kSubToAdd = "SubToAdd";
constr kSubToAdd_p = "SubToAdd_p";
constr kTokenize = "Tokenize";
constr kTokenize_p = "Tokenize_p";
constr kRPN = "RPN";
constr kRPN_p = "RPN_p";
constr kCalc = "Calc";
constr kCalc_p = "Calc_p";
constr kFractions = "Fractions";
constr kLog = "Log";
constr kColor = "Color";

} // namespace Config

} // namespace Math



#endif /* CONFIG_H_ */
