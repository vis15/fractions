/*
 * fraction.h
 *
 *  Created on: Nov 2, 2013
 *      Author: dis
 */

#ifndef FRACTION_H_
#define FRACTION_H_

#include <vector>

#include "vars.h"

namespace Math
{

class Fraction //Fractions cannot be of type 1.2/5.6
{
private: //variables

public: //variables
	longl numerator_;
	longl denominator_;
	
private: //functions

public:
	Fraction();
	Fraction(clong numerator, clong denominator);
	Fraction(constr& fraction); //needs to be done
	~Fraction();
	Fraction operator+ (const Fraction& num);
	Fraction operator- (const Fraction& num);
	Fraction operator* (const Fraction& num);
	Fraction operator/ (const Fraction& num);
	Fraction operator^ (const Fraction& num);
	clong getCommonDenominator (clong num1, clong num2); //get common denominator
	Sout<Fraction, Fraction> setCommonDenominator (const Fraction& num1, const Fraction& num2); //set common denominator for both fractions
	constr toString(void);
	Fraction setNegitive(const Fraction& num); //function will check if denominator is negative, if so change it to numerator
	static bool isFraction(constr& fraction);
	static cvstr fractionSplit(constr& fraction);
	void setNumerator(clongl num);
	void setDenominator(clongl num);
	longl toStoll(constr& num);
};

typedef std::vector<Math::Fraction> vecfract;

} //namespace Math
#endif /* FRACTION_H_ */
