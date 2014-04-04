/*
 * version.cpp
 *
 *  Created on: Jan 10, 2014
 *      Author: dis
 */

#include "version.h"

namespace Math
{

namespace Version
{

constr buildType(VersionType type)
{
	switch(type)
	{
	case VersionType::kAlpha:
		return "Alpha";
	case VersionType::kBeta:
		return "Beta";
	case VersionType::kStable:
		return "Stable";
	default:
		return "";
	}
}

} // namespace Version

} // namespace Math


