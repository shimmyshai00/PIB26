/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      compare.cpp
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
	int BigInt::compare(const BigInt &rhs) const {
		if(m_sign > rhs.m_sign) {
			return +1;
		} else if(m_sign < rhs.m_sign) {
			return -1;
		} else {
			return ucompare(rhs);
		}
	}

	int BigInt::compare(int smallNum) const {
		if((m_sign == SIGN_POSITIVE) && (smallNum < 0)) {
			return +1;
		} else if((m_sign == SIGN_NEGATIVE) && (smallNum >= 0)) {
			return -1;
		} else {
			if(smallNum >= 0) {
				return ucompare(smallNum);
			} else {
				return ucompare(-smallNum);
			}
		}
	}

	int BigInt::compare(unsigned int smallNum) const {
		if(m_sign == SIGN_NEGATIVE) {
			return -1;
		} else {
			return ucompare(smallNum);
		}
	}
}
