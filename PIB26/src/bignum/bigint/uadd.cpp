/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      uadd.cpp
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

#include "../primitives/add.hpp"

namespace SDF::Bignum {
	void BigInt::uadd(const BigInt &num1, const BigInt &num2) {
		// Crop num1 and num2 to fit if they are too large (wraparound overflow).
		std::size_t num1Length(std::min(num1.m_digitsUsed, m_digitsAlloc));
		std::size_t num2Length(std::min(num2.m_digitsUsed, m_digitsAlloc));

		// Handle each length case.
		Digit carry(0);
		if(num1Length >= num2Length) {
			// Case: AAAAAAAAAAAAAAAAAA
			// (MSD)         BBBBBBBBBB (LSD)
			// A = num1, B = num2
			std::size_t num1Overhang(num1Length - num2Length);

			carry = Primitives::add(m_digits, num1.m_digits, num2.m_digits, num2Length, carry);
			carry = Primitives::propagateCarry(m_digits + num2Length, num1.m_digits + num2Length, carry, num1Overhang);

			m_digitsUsed = num1Length;
		} else {
			// Case:         AAAAAAAAAA
			//       BBBBBBBBBBBBBBBBBB
			uadd(num2, num1);
		}

		// Handle leftover carry and compute length of result.
		if(carry) {
			m_digits[m_digitsUsed] = carry;
			++m_digitsUsed;
		}

		m_sign = SIGN_POSITIVE;
	}

	void BigInt::uaddIp(const BigInt &num) {
		// In this case, the above code also works just as well for in-place additions.
		uadd(*this, num);
	}
}
