/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      recip.cpp
 * Timestamp: Mar 19, 2021
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

#include <iostream>
#include <cmath>

namespace SDF::Bignum
{
	void BigFloat::recip(BigFloat &a, Bignum::IMultiplicationStrategy &strategy,
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

		// Create an initial guess by finding the reciprocal of a's first digit.
		m_sign = a.m_sign;
		double aRecip(1.0 / a.m_digits[a.m_totalLen - 1]);
		m_digits[m_totalLen - 1] = floor(aRecip * Bignum::BASE);
		m_exp = -(a.m_exp + 1);

		// Perform a few initial iterations at low precision to maximize the precision of this
		// guess.
		BigFloat two(2);
		two.assign(2);

		ticker->setTickerMax(origPrec * DIGS_PER_DIG);
		ticker->printTicker();

		for (std::size_t i(0); i < 2 * DIGS_PER_DIG; ++i) {
			tmpValReduced.mul(aReduced, *this, strategy);
			tmpValReduced.subIp(two);
			tmpValReduced.m_sign = static_cast<Sign>(-tmpValReduced.m_sign);
			mul(*this, tmpValReduced, strategy);
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
			//     x_(n+1) = x_n (2 - a x_n).
			//
			ticker->setTickerCur(prec * DIGS_PER_DIG);
			ticker->printTicker();

			// We can make this computation more efficient by computing it as instead
			//
			//     x_(n+1) = 2 x_n - a (x_n^2)
			//
			// where the squaring can then be done with half precision inputs, in addition to that
			// squarings are easier with some multiplication algorithms.
			tmpValReduced.sqr(*this, strategy); // does mul at previous prec
			resize(prec); // upgrade to full prec

			tmpValReduced.mul(aReduced, tmpValReduced, strategy);
			mulIp(2);
			subIp(tmpValReduced);
		} while (prec < origPrec);

		ticker->finishTicker();

		// Restore original pointers.
		resize(origPrec);
	}
}
