/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      mul.cpp
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

namespace SDF::Bignum
{
	void BigFloat::mul(const BigFloat &num1, const BigFloat &num2, IMultiplicationStrategy &strategy)
	{
		Sign n1Sign(num1.m_sign), n2Sign(num2.m_sign);
		umul(num1, num2, strategy);
		m_sign = static_cast<Sign>(n1Sign * n2Sign);
	}

	void BigFloat::sqr(const BigFloat &num, IMultiplicationStrategy &strategy) {
		usqr(num, strategy);
	}

	void BigFloat::mul(const BigFloat &num1, const BigInt &num2, IMultiplicationStrategy &strategy)
	{
		Sign n1Sign(num1.m_sign), n2Sign(num2.m_sign);
		umul(num1, num2, strategy);
		m_sign = static_cast<Sign>(n1Sign * n2Sign);
	}
}
