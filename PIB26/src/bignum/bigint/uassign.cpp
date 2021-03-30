/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      uassign.cpp
 * Timestamp: Mar 13, 2021
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

namespace SDF::Bignum
{
	// Private members.
	void BigInt::uassign(unsigned int smallNum)
	{
		m_sign = SIGN_POSITIVE;
		m_digitsUsed = Primitives::assignSmall(m_digits, smallNum, m_digitsAlloc);
	}

	void BigInt::uassign(const BigInt &rhs)
	{
		m_sign = SIGN_POSITIVE;

		// Handle the different length cases involved here.
		std::size_t rMax(m_digitsAlloc);
		std::size_t aSize(rhs.m_digitsUsed);
		if(rMax >= aSize) {
			// We have enough room to just copy the digits straight up.
			m_digitsUsed = aSize;
			Primitives::copy(m_digits, rhs.m_digits, aSize);
		} else {
			// We have to chop rhs (wraparound overflow).
			m_digitsUsed = rMax;
			Primitives::copy(m_digits, rhs.m_digits, rMax);
		}
	}
}

