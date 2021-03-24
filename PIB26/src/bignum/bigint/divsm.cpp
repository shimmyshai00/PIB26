/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      divsm.cpp
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

namespace SDF::Bignum
{
	void BigInt::div(const BigInt &num1, int smallNum)
	{
		if (smallNum >= 0) {
			udiv(num1, smallNum);
			m_sign = num1.m_sign;
		} else {
			udiv(num1, -smallNum);
			m_sign = static_cast<Sign>(-num1.m_sign);
		}
	}

	void BigInt::div(const BigInt &num1, unsigned int smallNum)
	{
		udiv(num1, smallNum);
		m_sign = num1.m_sign;
	}

	void BigInt::divIp(int smallNum)
	{
		if (smallNum >= 0) {
			Sign curSign(m_sign);
			udivIp(smallNum);
			m_sign = curSign;
		} else {
			Sign curSign(m_sign);
			udivIp(-smallNum);
			m_sign = static_cast<Sign>(-m_sign);
		}
	}

	void BigInt::divIp(unsigned int smallNum)
	{
		Sign curSign(m_sign);
		udivIp(smallNum);
		m_sign = curSign;
	}
}
