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
	void init(constr& working_dir, const bool debug);
	
	static constr kFileName_;
	static constr kFileExt_;
	str kConfigFile_;
	ConfigSettings config_;
	
private:
	void parseVars(constr varname, constr var);
	
	bool debug_;
};

namespace ConfigVar
{

constr kWindowSizeL = "WindowSizeL";
constr kWindowSizeW = "WindowSizeW";
constr kWindowPositionX = "WindowPositionX";
constr kWindowPositionY = "WindowPositionY";

} // namespace Config

} // namespace Math



#endif /* CONFIG_H_ */
