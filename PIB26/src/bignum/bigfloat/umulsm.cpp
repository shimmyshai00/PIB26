/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      umulsm.cpp
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

#include "../primitives/assign.hpp"
#include "../primitives/compare.hpp"
#include "../primitives/mulsm.hpp"
#include "../primitives/shift.hpp"

namespace SDF::Bignum
{
	void BigFloat::umul(const BigFloat &num1, unsigned int smallNum)
	{
		if (smallNum == 0) {
			// Special case.
			m_sign = SIGN_POSITIVE;
			m_exp = 0;
			Primitives::zeroize(m_digits, m_totalLen);
		} else {
			m_sign = SIGN_POSITIVE;
			m_exp = num1.m_exp;

			// Handle the different length cases.
			TwoDigit bigCarry(0);
			if (m_totalLen >= num1.m_totalLen) {
				std::size_t excess(m_totalLen - num1.m_totalLen);
				Primitives::zeroize(m_digits, excess);
				bigCarry = Primitives::mulBySmall(m_digits + excess, num1.m_digits, smallNum,
					num1.m_totalLen);
			} else {
				std::size_t crop(num1.m_totalLen - m_totalLen);
				bigCarry = Primitives::mulBySmall(m_digits, num1.m_digits + crop, smallNum,
					m_totalLen);
			}

			// Now handle the remaining carry.
			if (bigCarry) {
				std::size_t carryDigits(Primitives::countSignifDigits(bigCarry));
				Primitives::rshift(m_digits, carryDigits, m_totalLen);
				Primitives::assignSmall(m_digits + (m_totalLen - carryDigits), bigCarry, carryDigits);

				m_exp += carryDigits;
			}
		}
	}

	void BigFloat::umulIp(unsigned int smallNum)
	{
		if (smallNum == 0) {
			// Special case.
			m_sign = SIGN_POSITIVE;
			m_exp = 0;
			Primitives::zeroize(m_digits, m_totalLen);
		} else {
			m_sign = SIGN_POSITIVE;

			// Only one length case.
			TwoDigit bigCarry(0);
			bigCarry = Primitives::mulBySmall(m_digits, m_digits, smallNum, m_totalLen);

			// Now handle the remaining carry.
			if (bigCarry) {
				std::size_t carryDigits(Primitives::countSignifDigits(bigCarry));
				Primitives::rshift(m_digits, carryDigits, m_totalLen);
				Primitives::assignSmall(m_digits + (m_totalLen - carryDigits), bigCarry, carryDigits);

				m_exp += carryDigits;
			}
		}
	}
}
