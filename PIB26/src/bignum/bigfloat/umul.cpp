/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      umul.cpp
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

#include "../IMultiplicationStrategy.hpp"

#include "../primitives/assign.hpp"

namespace SDF::Bignum
{
	void BigFloat::umul(const BigFloat &num1, const BigFloat &num2,
		IMultiplicationStrategy &strategy)
	{
		m_sign = SIGN_POSITIVE;
		m_exp = num1.m_exp + num2.m_exp;

		strategy.mulDigits(num1.m_digits, num1.m_totalLen, num2.m_digits, num2.m_totalLen);
		std::size_t prodLen(strategy.getProductLength());
		if (prodLen <= m_totalLen) {
			// We can store the whole product.
			strategy.getProductDigits(m_digits + (m_totalLen - prodLen), 0, prodLen);
			Primitives::zeroize(m_digits, m_totalLen - prodLen);
		} else {
			// We can only store the upper part of the product.
			strategy.getProductDigits(m_digits, prodLen - m_totalLen, m_totalLen);
		}

		if (prodLen == num1.m_totalLen + num2.m_totalLen) {
			++m_exp;
		}
	}

	void BigFloat::umul(const BigFloat &num1, const BigInt &num2, IMultiplicationStrategy &strategy)
	{
		m_sign = SIGN_POSITIVE;
		if (num2.m_digitsUsed == 0) {
			uassign(0);
		} else {
			m_exp = num1.m_exp + (num2.m_digitsUsed - 1);

			strategy.mulDigits(num1.m_digits, num1.m_totalLen, num2.m_digits, num2.m_digitsUsed);
			std::size_t prodLen(strategy.getProductLength());
			if (prodLen <= m_totalLen) {
				// We can store the whole product.
				strategy.getProductDigits(m_digits + (m_totalLen - prodLen), 0, prodLen);
				Primitives::zeroize(m_digits, m_totalLen - prodLen);
			} else {
				// We can only store the upper part of the product.
				strategy.getProductDigits(m_digits, prodLen - m_totalLen, m_totalLen);
			}

			if (prodLen == num1.m_totalLen + num2.m_digitsUsed) {
				++m_exp;
			}
		}
	}
}
