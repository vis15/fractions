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
		return "<b>Alpha</b>";
	case VersionType::kBeta:
		return "<b>Beta</b>";
	case VersionType::kStable:
		return "<b>Stable</b>";
	default:
		return "";
	}
}

} // namespace Version

} // namespace Math


