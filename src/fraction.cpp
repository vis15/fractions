/*
 * fraction.cpp
 *
 *  Created on: Nov 2, 2013
 *      Author: dis
 */

#include <cmath>

#include "fraction.h"
#include "util.h"

namespace Math
{

Fraction Fraction::operator+ (const Fraction& num)
{
	const Fraction new_this = Fraction::setNegitive(*this);
	const Fraction new_num = Fraction::setNegitive(num);
	Sout<Fraction, Fraction> out = Fraction::setCommonDenominator(new_this, new_num);

	Fraction output;
	output.numerator_ = out.out1.numerator_+out.out2.numerator_;
	output.denominator_ = out.out1.denominator_;

	return output;
}

Fraction Fraction::operator- (const Fraction& num)
{
	Fraction new_num(num);

	new_num.numerator_ = num.numerator_*-1;

	//Fraction output = *this+new_num;

	return *this+new_num;
}

clong Fraction::getCommonDenominator(clong num1, clong num2)
{
	if(num1 == num2) return num1;

	int max = fmax(num1, num2);
	int min = fmin(num1, num2);

	if(max % min == 0) //is max a multiple of min? if yes
	{
		return max;
	}
	else
	{
		return max*min;
	}
}

Fraction::Fraction()
{
	numerator_ = 0;
	denominator_ = 0;
}

Fraction::Fraction(clong numerator, clong denominator)
{
	numerator_ = numerator;
	denominator_ = denominator;
}

Fraction::~Fraction()
{
	//destructor
}

//Two Fractions num1, a/b, and num2, c/d
Sout<Fraction, Fraction> Fraction::setCommonDenominator(const Fraction& num1, const Fraction& num2)
{
	Sout<Fraction, Fraction> output;
	Fraction out1(num1);
	Fraction out2(num2);

	if(num1.denominator_ == num2.denominator_)
	{
		output.out1 = num1;
		output.out2 = num2;
		return output;
	}

	if(num2.denominator_ > num1.denominator_)
	{
		out1 = num2;
		out2 = num1;
	}

	if(out1.denominator_ % out2.denominator_ == 0) //if b and d are multiples of each other then c/d = c/d * (b/d)/(b/d)
	{
		clong multi = out1.denominator_/out2.denominator_;
		out2.numerator_ = out2.numerator_*multi;
		out2.denominator_ = out2.denominator_*multi;
		output.out1 = out1;
		output.out2 = out2;

		return output;
	}
	else //if b and d are not multiples of each other then a/b = a/b * d/d and c/d = c/d * b/b
	{
		int out1_d = out1.denominator_;
		out1.numerator_ = out1.numerator_*out2.denominator_;
		out1.denominator_ = out1.denominator_*out2.denominator_;
		out2.numerator_ = out2.numerator_*out1_d;
		out2.denominator_ = out2.denominator_*out1_d;

		output.out1 = out1;
		output.out2 = out2;

		return output;
	}
}

constr Fraction::toString()
{
	return Util::toString(numerator_) + "/" + Util::toString(denominator_);
}

Fraction Fraction::setNegitive(const Fraction& num)
{
	Fraction output;

	if(num.denominator_ < 0)
	{
		output.denominator_ = num.denominator_*-1;
		output.numerator_ = num.numerator_*-1;

		return output;
	}

	return num;
}

} //namespace Math
