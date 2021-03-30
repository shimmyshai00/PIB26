/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      udivsm.hpp
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

#include "../primitives/shift.hpp"
#include "../primitives/divsm.hpp"

namespace SDF::Bignum
{
	void BigFloat::udiv(const BigFloat &num1, unsigned int smallNum)
	{
		// We have a primitive just for this.
		std::size_t expDec(
			Primitives::fpDivBySmall(m_digits, num1.m_digits, smallNum, m_totalLen,
				num1.m_totalLen));
		m_sign = SIGN_POSITIVE;
		m_exp = num1.m_exp - expDec;
	}

	void BigFloat::udivIp(unsigned int smallNum)
	{
		std::size_t expDec(
			Primitives::fpDivBySmall(m_digits, m_digits, smallNum, m_totalLen, m_totalLen));
		m_sign = SIGN_POSITIVE;
		m_exp -= expDec;
	}
}
