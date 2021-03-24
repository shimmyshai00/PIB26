/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      add.cpp
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

namespace SDF::Bignum {
	void BigInt::add(const BigInt &num1, const BigInt &num2) {
		if(num1.m_sign == num2.m_sign) {
			// like signs add
			uadd(num1, num2);
			m_sign = num1.m_sign;
		} else {
			// unlike signs sub
			if(num1.m_sign == SIGN_POSITIVE) {
				usub(num1, num2);
			} else {
				usub(num2, num1);
			}
		}
	}

	void BigInt::addIp(const BigInt &num1) {
		if(m_sign == num1.m_sign) {
			// like signs add
			uaddIp(num1);
			m_sign = num1.m_sign;
		} else {
			// unlike signs sub
			if(m_sign == SIGN_POSITIVE) {
				usubIp(num1);
			} else {
				usubIp(num1);
				m_sign = static_cast<Sign>(-m_sign);
			}
		}
	}
}
