/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      FlexMul3.cpp
 * Timestamp: Mar 23, 2021
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

#include "FlexMul3.hpp"

namespace SDF::Bignum::Multiplication
{
	FlexMul3::FlexMul3(IMultiplicationStrategy *strategy1, IMultiplicationStrategy *strategy2,
		IMultiplicationStrategy *strategy3, std::size_t overrideLen1, std::size_t overrideLen2)
		: m_strategy1(strategy1), m_strategy2(strategy2), m_strategy3(strategy3), m_overrideLen1(
			overrideLen1), m_overrideLen2(overrideLen2), m_lastStrategy(nullptr)
	{
	}

	void FlexMul3::mulDigits(Memory::SafePtr<Digit> a, std::size_t aLen, Memory::SafePtr<Digit> b,
		std::size_t bLen)
	{
		std::size_t prodLen(aLen + bLen);

		if (prodLen < m_overrideLen1) {
			m_lastStrategy = m_strategy1;
		} else if (prodLen < m_overrideLen2) {
			m_lastStrategy = m_strategy2;
		} else {
			m_lastStrategy = m_strategy3;
		}

		m_lastStrategy->mulDigits(a, aLen, b, bLen);
	}

	void FlexMul3::squareDigits(Memory::SafePtr<Digit> a, std::size_t aLen) {
		std::size_t prodLen(aLen << 1);

		if (prodLen < m_overrideLen1) {
			m_lastStrategy = m_strategy1;
		} else if (prodLen < m_overrideLen2) {
			m_lastStrategy = m_strategy2;
		} else {
			m_lastStrategy = m_strategy3;
		}

		m_lastStrategy->squareDigits(a, aLen);
	}

	std::size_t FlexMul3::getProductLength() const
	{
		if (m_lastStrategy != nullptr) {
			return m_lastStrategy->getProductLength();
		} else {
			return 0;
		}
	}

	void FlexMul3::getProductDigits(Memory::SafePtr<Digit> dst, std::size_t origin,
		std::size_t length)
	{
		if (m_lastStrategy != nullptr) {
			m_lastStrategy->getProductDigits(dst, origin, length);
		}
	}
}

