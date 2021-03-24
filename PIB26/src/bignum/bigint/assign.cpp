/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      assign.cpp
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

namespace SDF::Bignum {
	void BigInt::assign(int smallNum) {
		if(smallNum >= 0) {
			uassign(smallNum);
			m_sign = SIGN_POSITIVE;
		} else {
			uassign(-smallNum);
			m_sign = SIGN_NEGATIVE;
		}
	}

	void BigInt::assign(unsigned int smallNum) {
		uassign(smallNum);
		m_sign = SIGN_POSITIVE;
	}

	void BigInt::assign(const BigInt &rhs) {
		uassign(rhs);
		m_sign = rhs.m_sign;
	}
}
