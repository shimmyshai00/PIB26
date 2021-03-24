/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      ClassicalSmallMul.cpp
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

#include "ClassicalSmallMul.hpp"

namespace SDF::Bignum::Multiplication
{
	ClassicalSmallMul::ClassicalSmallMul(std::size_t maxProdSize)
		: m_lastProductLength(0), m_productBuffer(maxProdSize)
	{
	}

	void ClassicalSmallMul::mulDigits(Memory::SafePtr<Digit> a, std::size_t aLen,
		Memory::SafePtr<Digit> b, std::size_t bLen)
	{
		// Implementation of the classic "grade school" multiplication method.
		std::size_t prodLen(aLen + bLen);
		Memory::SafePtr<Digit> prodBufPtr(m_productBuffer.accessData(0));

		// Zeroize the relevant part of the product buffer.
		for (std::size_t i(0); i < prodLen; ++i) {
			prodBufPtr[i] = 0;
		}

		// Do the multiplication.
		for (std::size_t j(0); j < bLen; ++j) {
			// Perform a fused multiply and add of a with a digit of b and then adding it to the
			// product buffer, shifted left by the appropriate amount.
			TwoDigit carry(0);
			for (std::size_t i(0); i < aLen; ++i) {
				TwoDigit tmp(prodBufPtr[j + i] + (static_cast<TwoDigit>(a[i]) * b[j]) + carry);
				carry = tmp / BASE;
				tmp -= carry * BASE;
				prodBufPtr[j + i] = static_cast<Digit>(tmp);
			}

			// Slop the remaining carry over into the next digit if needed. The largest possible
			// carry is always, given that in each step we do a proper, canonical addition to the
			// preceding sum of partial products, ((BASE-1) + (BASE-1)^2)/BASE = BASE-1, so we don't
			// need to worry about further expansion of the carry than this.
			prodBufPtr[j + aLen] = carry;
		}

		// Get the length of the product. The actual length may be up to 1 digit less than the one
		// given as prodLen above if the multiplier is suitably small. (It could also be zero, if the
		// a factor was zero, but to make efficient we don't check this - NOTE! CAVEAT! this is
		// delegated to the bignum type implementation!)
		if (prodBufPtr[prodLen - 1] != 0) {
			m_lastProductLength = prodLen;
		} else {
			m_lastProductLength = prodLen - 1;
		}
	}

	std::size_t ClassicalSmallMul::getProductLength() const
	{
		return m_lastProductLength;
	}

	void ClassicalSmallMul::getProductDigits(Memory::SafePtr<Digit> dst, std::size_t origin,
		std::size_t length)
	{
		Memory::SafePtr<Digit> prodBufPtr(m_productBuffer.accessData(origin));
		for (std::size_t i(0); i < length; ++i) {
			dst[i] = prodBufPtr[i];
		}
	}
}
