/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      uassign.cpp
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

#include "../BigInt.hpp"

#include "../primitives/assign.hpp"
#include "../primitives/compare.hpp"

namespace SDF::Bignum
{
	void BigFloat::uassign(unsigned int smallNum)
	{
		if (smallNum == 0) {
			// Special case.
			m_sign = SIGN_POSITIVE;
			m_exp = 0;
			Primitives::zeroize(m_digits, m_totalLen);
		} else {
			m_sign = SIGN_POSITIVE;

			std::size_t signif(Primitives::countSignifDigits(smallNum));
			m_exp = signif - 1;
			Primitives::assignSmall(m_digits + (m_totalLen - signif), smallNum, signif);
		}
	}

	void BigFloat::uassign(const BigFloat &rhs)
	{
		m_sign = SIGN_POSITIVE;
		m_exp = rhs.m_exp;

		// There are two length cases here to consider, depending on whether we have enough room to
		// store all of rhs or not.
		if (rhs.m_totalLen <= m_totalLen) {
			// We have enough room.
			std::size_t excess(m_totalLen - rhs.m_totalLen);
			Primitives::zeroize(m_digits, excess);
			Primitives::copy(m_digits + excess, rhs.m_digits, rhs.m_totalLen);
		} else {
			// Not enough room. Crop rhs.
			std::size_t crop(rhs.m_totalLen - m_totalLen);
			Primitives::copy(m_digits, rhs.m_digits + crop, m_totalLen);
		}
	}

	void BigFloat::uassign(const BigInt &rhs)
	{
		// Like the above, except we have to figure the exponent differently.
		if (rhs.m_digitsUsed == 0) {
			// Special case.
			m_sign = SIGN_POSITIVE;
			m_exp = 0;
			Primitives::zeroize(m_digits, m_totalLen);
		} else {
			m_sign = SIGN_POSITIVE;
			m_exp = rhs.m_digitsUsed - 1;

			// There are two length cases here to consider, depending on whether we have enough room to
			// store all of rhs or not.
			if (rhs.m_digitsUsed <= m_totalLen) {
				// We have enough room.
				std::size_t excess(m_totalLen - rhs.m_digitsUsed);
				Primitives::zeroize(m_digits, excess);
				Primitives::copy(m_digits + excess, rhs.m_digits, rhs.m_digitsUsed);
			} else {
				// Not enough room. Crop rhs.
				std::size_t crop(rhs.m_digitsUsed - m_totalLen);
				Primitives::copy(m_digits, rhs.m_digits + crop, m_totalLen);
			}
		}
	}
}

