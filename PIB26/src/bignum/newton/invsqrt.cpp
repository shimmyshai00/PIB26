/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      invsqrt.cpp
 * Timestamp: Mar 20, 2021
 *
 */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../BigFloat.hpp"

#include "../../util/ITicker.hpp"
#include "../../util/LabelTicker.hpp"

#include <iostream>
#include <cmath>

namespace SDF::Bignum
{
	void BigFloat::invsqrt(BigFloat &a, Bignum::IMultiplicationStrategy &strategy,
		Util::ITicker *ticker)
	{
		assign(0);

		// Create a temporary buffer for the Newton calculations.
		BigFloat tmpVal(m_precNominal);
		std::size_t prec(2);

		// Set up some reduced-precision versions of our BigFloats.
		std::size_t origPrec(m_precNominal);

		BigFloat aReduced;
		BigFloat tmpValReduced;

		resize(prec);
		aReduced = a.aliasTruncate(prec);
		tmpValReduced = tmpVal.aliasTruncate(prec);

		// Create an initial guess in r by the following formula:
		//    1/sqrt(f * BASE^exp) = 1/sqrt(f) * BASE^(-1/2 * exp)
		m_sign = a.m_sign;
		double aRecipSqrt(1.0 / sqrt(a.m_digits[a.m_totalLen - 1]));
		if (a.m_exp % 2 == 0) {
			m_digits[m_totalLen - 1] = floor(aRecipSqrt * Bignum::BASE);
			m_exp = -(a.m_exp / 2 + 1);
		} else {
			aRecipSqrt = aRecipSqrt / sqrt(BASE);
			m_digits[m_totalLen - 1] = floor(aRecipSqrt * Bignum::BASE);
			m_exp = -((a.m_exp - 1) / 2 + 1);
		}

		// Perform a few initial iterations at low precision to maximize the precision of this
		// guess.
		BigFloat three(2);
		three.assign(3);

		ticker->setTickerMax(origPrec * DIGS_PER_DIG);
		ticker->printTicker();

		for (std::size_t i(0); i < 2 * DIGS_PER_DIG; ++i) {
			tmpValReduced.sqr(*this, strategy); // n.b. could have special squaring method
			tmpValReduced.mul(aReduced, *this, strategy);
			tmpValReduced.subIp(three);
			tmpValReduced.m_sign = static_cast<Sign>(-m_sign);
			mul(*this, tmpValReduced, strategy);
			divIp(2);
		}

		// Perform the remaining iterations.
		do {
			prec <<= 1;
			if (prec > origPrec) {
				prec = origPrec;
			}

			aReduced = a.aliasTruncate(prec);
			tmpValReduced = tmpVal.aliasTruncate(prec);

			// The Newton formula is:
			//
			//     x_(n+1) = x_n (3 - a x_n^2)/2.
			//
			ticker->setTickerCur(prec * DIGS_PER_DIG);
			ticker->printTicker();

			// again, we can do this mul at half prec
			tmpValReduced.sqr(*this, strategy); // n.b. could have special squaring method
			resize(prec);

			tmpValReduced.mul(aReduced, tmpValReduced, strategy);
			tmpValReduced.subIp(three);
			tmpValReduced.m_sign = static_cast<Sign>(-tmpValReduced.m_sign);
			mul(*this, tmpValReduced, strategy);
			divIp(2);
		} while (prec < origPrec);

		ticker->finishTicker();

		// Restore original pointers.
		resize(origPrec);
	}

	void BigFloat::invsqrt(unsigned int a, Bignum::IMultiplicationStrategy &strategy,
		Util::ITicker *ticker)
	{
		assign(0);

		// Create a temporary buffer for the Newton calculations.
		BigFloat tmpVal(m_precNominal);
		std::size_t prec(2);

		// Set up some reduced-precision versions of our BigFloats.
		std::size_t origPrec(m_precNominal);
		BigFloat tmpValReduced;

		resize(prec);
		tmpValReduced = tmpVal.aliasTruncate(prec);

		// Create an initial guess in r.
		double aRecipSqrt(sqrt(1.0 / a));

		m_sign = SIGN_POSITIVE;
		m_exp = 0;
		while (aRecipSqrt < 1.0f) {
			aRecipSqrt *= BASE;
			--m_exp;
		}
		m_digits[m_totalLen - 1] = floor(aRecipSqrt);

		// Perform a few initial iterations at low precision to maximize the precision of this
		// guess.
		BigFloat three(2);
		three.assign(3);

		ticker->setTickerMax(origPrec * DIGS_PER_DIG);
		ticker->printTicker();

		for (std::size_t i(0); i < 2 * DIGS_PER_DIG; ++i) {
			tmpValReduced.sqr(*this, strategy); // n.b. could have special squaring method
			tmpValReduced.mulIp(a);
			tmpValReduced.subIp(three);
			tmpValReduced.m_sign = static_cast<Sign>(-tmpValReduced.m_sign);
			mul(*this, tmpValReduced, strategy);
			divIp(2);
		}

		// Perform the remaining iterations.
		do {
			prec <<= 1;
			if (prec > origPrec) {
				prec = origPrec;
			}

			tmpValReduced = tmpVal.aliasTruncate(prec);

			// The Newton formula is:
			//
			//     x_(n+1) = x_n (3 - a x_n^2)/2.
			//
			ticker->setTickerCur(prec * DIGS_PER_DIG);
			ticker->printTicker();

			tmpValReduced.sqr(*this, strategy); // n.b. could have special squaring method
			resize(prec);

			tmpValReduced.mulIp(a);
			tmpValReduced.subIp(three);
			tmpValReduced.m_sign = static_cast<Sign>(-tmpValReduced.m_sign);
			mul(*this, tmpValReduced, strategy);
			divIp(2);
		} while (prec < origPrec);

		ticker->finishTicker();

		// Restore original pointers.
		resize(origPrec);
	}
}
