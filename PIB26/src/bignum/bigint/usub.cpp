/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      usub.cpp
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

#include "../primitives/compare.hpp"
#include "../primitives/sub.hpp"

#include <iostream>

namespace SDF::Bignum
{
	void BigInt::usub(const BigInt &num1, const BigInt &num2)
	{
		// Crop num1 and num2 to fit if they are too large (wraparound overflow).
		std::size_t num1Length(std::min(num1.m_digitsUsed, m_digitsAlloc));
		std::size_t num2Length(std::min(num2.m_digitsUsed, m_digitsAlloc));

		// Handle each length case.
		Digit borrow(0);
		if (num1Length >= num2Length) {
			// Case: AAAAAAAAAAAAAAAAAA
			// (MSD)         BBBBBBBBBB (LSD)
			// A = num1, B = num2
			std::size_t num1Overhang(num1Length - num2Length);

			borrow = Primitives::sub(m_digits, num1.m_digits, num2.m_digits, num2Length, borrow);
			borrow = Primitives::propagateBorrow(m_digits + num2Length, num1.m_digits + num2Length, borrow,
				num1Overhang);

			m_digitsUsed = num1Length;
		} else {
			// Case:         AAAAAAAAAA
			//       BBBBBBBBBBBBBBBBBB
			std::size_t num2Overhang(num2Length - num1Length);

			borrow = Primitives::sub(m_digits, num1.m_digits, num2.m_digits, num1Length, borrow);
			borrow = Primitives::neg(m_digits + num1Length, num2.m_digits + num1Length, borrow, num2Overhang);

			m_digitsUsed = num2Length;
		}

		// Handle remaining borrow - if there is a borrow out, the result is negative and needs
		// negating.
		if (borrow) {
			Primitives::neg(m_digits, m_digits, 0, m_digitsUsed);
			m_sign = SIGN_NEGATIVE;
		} else {
			m_sign = SIGN_POSITIVE;
		}

		// Correct for any possible cancellation.
		m_digitsUsed = Primitives::countSignifDigits(m_digits, m_digitsUsed);
	}

	void BigInt::usubIp(const BigInt &num)
	{
		// The same code above also works in-place.
		usub(*this, num);
	}
}
