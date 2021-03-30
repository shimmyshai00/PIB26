/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      umul.cpp
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

#include "../IMultiplicationStrategy.hpp"

namespace SDF::Bignum
{
	void BigInt::umul(const BigInt &num1, const BigInt &num2, IMultiplicationStrategy &strategy)
	{
		// The multiplication strategy algorithm takes care of all length cases.
		strategy.mulDigits(num1.m_digits, num1.m_digitsUsed, num2.m_digits, num2.m_digitsUsed);

		// Check if we have enough room to hold the full product. If not, crop it (wraparound
		// overflow).
		if (strategy.getProductLength() <= m_digitsAlloc) {
			m_digitsUsed = strategy.getProductLength();
			strategy.getProductDigits(m_digits, 0, m_digitsUsed);
		} else {
			m_digitsUsed = m_digitsAlloc;
			strategy.getProductDigits(m_digits, 0, m_digitsAlloc);
		}

		// Correct for possible cancellation and set sign.
		m_digitsUsed = Primitives::countSignifDigits(m_digits, m_digitsUsed);
		m_sign = SIGN_POSITIVE;
	}
}
