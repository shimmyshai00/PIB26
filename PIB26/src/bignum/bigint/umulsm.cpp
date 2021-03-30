/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      umulsm.cpp
 * Timestamp: Mar 16, 2021
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

#include "../BigInt.hpp"

#include "../primitives/assign.hpp"
#include "../primitives/mulsm.hpp"

namespace SDF::Bignum
{
	void BigInt::umul(const BigInt &num1, unsigned int smallNum)
	{
		if (smallNum == 0) {
			// Special case.
			m_sign = SIGN_POSITIVE;
			m_digitsUsed = 0;
		} else {
			// Crop num1 to fit in the result if necessary (wraparound overflow).
			std::size_t num1Size(std::min(num1.m_digitsUsed, m_digitsAlloc));

			// Do the multiplication.
			TwoDigit carry(Primitives::mulBySmall(m_digits, num1.m_digits, smallNum, num1Size));
			m_digitsUsed = num1.m_digitsUsed;

			if (carry) {
				m_digitsUsed += Primitives::assignSmall(m_digits + m_digitsUsed, carry,
					m_digitsAlloc - m_digitsUsed);
			}

			m_sign = SIGN_POSITIVE;
		}
	}

	void BigInt::umulIp(unsigned int smallNum)
	{
		// The same code as above should also work in-place.
		umul(*this, smallNum);
	}
}
