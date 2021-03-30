/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      udivsm.cpp
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
#include "../primitives/compare.hpp"
#include "../primitives/divsm.hpp"

namespace SDF::Bignum
{
	void BigInt::udiv(const BigInt &num1, unsigned int smallNum)
	{
		// In the case of the small div, we can't just crop num1 to get an accurate wraparound result
		// - digits above still matter. So we have to handle length cases.
		if (m_digitsAlloc >= num1.m_digitsUsed) {
			// Case: RRRRRRRRRRRRRRRRR
			//              AAAAAAAAAA

			// No wraparound required.
			Primitives::divBySmall(m_digits, num1.m_digits, smallNum, num1.m_digitsUsed, 0);
			m_digitsUsed = num1.m_digitsUsed;
		} else {
			// Case:        RRRRRRRRRR
			//       AAAAAAAAAAAAAAAAA

			// Wraparound required. Modulo the overhanging part of num1 first.
			std::size_t aOverhang(num1.m_digitsUsed - m_digitsAlloc);
			TwoDigit remainder(
				Primitives::modBySmall(num1.m_digits + m_digitsAlloc, smallNum, aOverhang));
			Primitives::divBySmall(m_digits, num1.m_digits, smallNum, m_digitsAlloc, remainder);

			m_digitsUsed = m_digitsAlloc;
		}

		// Now correct for any shrinkage.
		m_digitsUsed = Primitives::countSignifDigits(m_digits, m_digitsUsed);
	}

	void BigInt::udivIp(unsigned int smallNum)
	{
		// The same code as above *should* also work in-place.
		udiv(*this, smallNum);
	}
}
