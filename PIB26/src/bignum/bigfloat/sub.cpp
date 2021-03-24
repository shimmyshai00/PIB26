/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      sub.cpp
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

namespace SDF::Bignum {
	void BigFloat::sub(const BigFloat &num1, const BigFloat &num2) {
		if((num1.m_sign == SIGN_POSITIVE) && (num2.m_sign == SIGN_POSITIVE)) {
			usub(num1, num2);
		} else if((num1.m_sign == SIGN_POSITIVE) && (num2.m_sign == SIGN_NEGATIVE)) {
			uadd(num1, num2);
		} else if((num1.m_sign == SIGN_NEGATIVE) && (num2.m_sign == SIGN_POSITIVE)) {
			uadd(num1, num2);
			m_sign = SIGN_NEGATIVE;
		} else if((num1.m_sign == SIGN_NEGATIVE) && (num2.m_sign == SIGN_NEGATIVE)) {
			usub(num2, num1);
		}
	}

	void BigFloat::subIp(const BigFloat &num1) {
		if((m_sign == SIGN_POSITIVE) && (num1.m_sign == SIGN_POSITIVE)) {
			usubIp(num1);
		} else if((m_sign == SIGN_POSITIVE) && (num1.m_sign == SIGN_NEGATIVE)) {
			uaddIp(num1);
		} else if((m_sign == SIGN_NEGATIVE) && (num1.m_sign == SIGN_POSITIVE)) {
			uaddIp(num1);
			m_sign = SIGN_NEGATIVE;
		} else if((m_sign == SIGN_NEGATIVE) && (num1.m_sign == SIGN_NEGATIVE)) {
			usubIp(num1);
			m_sign = static_cast<Sign>(-m_sign);
		}
	}
}
