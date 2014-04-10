/*
 * version.h
 *
 *  Created on: Jan 10, 2014
 *      Author: dis
 */

#ifndef VERSION_H_
#define VERSION_H_

#include "vars.h"

namespace Math
{

namespace Version
{

enum VersionType
{
	kAlpha, kBeta, kStable
};

inline constr kMajor() { return "0"; }
inline constr kMinor() { return "0"; }
inline constr kRevision() { return "0"; }
inline constr kBuild() { return "19"; }
constr buildType(VersionType);
inline constr kGetBuildType() { return buildType(kAlpha); }
inline constr kReleaseDate() { return __DATE__; }

} // namespace Version

} // namespace Math



#endif /* VERSION_H_ */
