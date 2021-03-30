/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      SmallKaratsuba.cpp
 * Timestamp: Mar 21, 2021
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

#include "SmallKaratsuba.hpp"

#include "../primitives/assign.hpp"
#include "../primitives/add.hpp"
#include "../primitives/sub.hpp"

#include <cstddef>
#include <algorithm>

#include <iostream>
#include <iomanip>

namespace SDF::Bignum::Multiplication
{
	// Note: It would be more efficient to give these multiplication strategies a common
	//       multiplication buffer instead of creating multiple buffers. Again, this is just a start
	//       to the full program.
	SmallKaratsuba::SmallKaratsuba(std::size_t maxProdSize)
		: m_lastProductLength(0), m_productBuffer(maxProdSize), m_workBuffer(
			maxProdSize + 2)
	{
	}

	void SmallKaratsuba::mulDigits(Memory::SafePtr<Digit> u, std::size_t uLen,
		Memory::SafePtr<Digit> v, std::size_t vLen)
	{
		// The Karatsuba method is based on the following identity:
		//
		//     (aB^n + b)(cB^n + d) = ac B^(2n) + (ad + bc) B^n + bd
		//
		// where B is the base, a, b, c, and d are "half"-sized segments of the input (such that the
		// two binomials on the right equal the two numbers to be multiplied), and n is the digit
		// length of half the largest input. We have the trick that this can actually be computed
		// with just 3 multiplications, by noting that ad + bc is part of (a + b)(c + d), which
		// generally equals ac + ad + bc + bd, so we can find ad + bc as (a + b)(c + d) - ac - bd,
		// and the last two are also what we need for the above.

		// Step 1: Zeroize the product buffer.
		Memory::SafePtr<Digit> prodBuf(m_productBuffer.accessData(0));
		for (std::size_t i(0); i < uLen + vLen; ++i) {
			prodBuf[i] = 0;
		}

		// Now go to the recursive multiplication.
		recursiveMul(prodBuf, u, uLen, v, vLen, m_workBuffer.accessData(0));
		m_lastProductLength = uLen + vLen;
		if (prodBuf[uLen + vLen - 1] == 0) {
			--m_lastProductLength;
		}
	}


	void SmallKaratsuba::squareDigits(Memory::SafePtr<Digit> a, std::size_t aLen) {
		// TBA
		mulDigits(a, aLen, a, aLen);
	}

	std::size_t SmallKaratsuba::getProductLength() const
	{
		return m_lastProductLength;
	}

	void SmallKaratsuba::getProductDigits(Memory::SafePtr<Digit> dst, std::size_t origin,
		std::size_t length)
	{
		Memory::SafePtr<Digit> prodBufPtr(m_productBuffer.accessData(origin));
		for (std::size_t i(0); i < length; ++i) {
			dst[i] = prodBufPtr[i];
		}
	}

	// Private member: The algorithm core.
	void SmallKaratsuba::recursiveMul(Memory::SafePtr<Digit> ans, Memory::SafePtr<Digit> u,
		std::size_t uLen, Memory::SafePtr<Digit> v, std::size_t vLen, Memory::SafePtr<Digit> work)
	{
		// Step 2: Make u the largest operand.
		if (uLen < vLen) {
			recursiveMul(ans, v, vLen, u, uLen, work);
		} else {
			// Step 3: Find the half size.
			std::size_t halfSize(uLen / 2);		// This is "n" in the above formula.

			for (std::size_t i(0); i < uLen + vLen; ++i) {
				ans[i] = 0;
				//work[i] = 0;
			}

			if (halfSize <= 2) {
				// Use a grade school mul to finish off the base case (note we should probably tweak
				// this to start this at larger sizes.) c.f. ClassicalSmallMul
				for (std::size_t j(0); j < vLen; ++j) {
					TwoDigit carry(0);
					for (std::size_t i(0); i < uLen; ++i) {
						TwoDigit tmp(ans[j + i] + (static_cast<TwoDigit>(u[i]) * v[j]) + carry);
						carry = tmp / BASE;
						tmp -= carry * BASE;
						ans[j + i] = static_cast<Digit>(tmp);
					}

					ans[j + uLen] = carry;
				}
			} else {
				// The next steps depend on whether the shorter operand was too small to be cut, given
				// that we cut both at the same digit significance.
				if (halfSize >= vLen) {
					// v is smaller than half. In this case, c = 0, and we have the product as just
					// ad B^n + bd.
					// Compute bd first, then ad.
					recursiveMul(ans, u, halfSize, v, vLen, work); // bd
					recursiveMul(work, u + halfSize, uLen - halfSize, v, vLen,
						work + (uLen + vLen - halfSize)); // ad

					// Do the addition.
					Primitives::add(ans + halfSize, ans + halfSize, work, uLen + vLen - halfSize, 0);
				} else {
					// v is larger than half. Now we need the full Karatsuba formula. First compute
					// the sums a + b and c + d.
					// Introduce some pointers for simplification and clarity.
					Memory::SafePtr<Digit> aPtr(u + halfSize);
					Memory::SafePtr<Digit> bPtr(u);
					Memory::SafePtr<Digit> cPtr(v + halfSize);
					Memory::SafePtr<Digit> dPtr(v);

					std::size_t aLen(uLen - halfSize);
					std::size_t bLen(halfSize);
					std::size_t cLen(vLen - halfSize);
					std::size_t dLen(halfSize);

					std::size_t sum1Len(std::max(aLen, bLen));
					std::size_t sum2Len(std::max(cLen, dLen));

					Memory::SafePtr<Digit> sum1Ptr(work); // a + b
					Memory::SafePtr<Digit> sum2Ptr(work + sum1Len); // c + d

					// Compute a + b.
					Digit sum1Carry(Primitives::unevenAdd(sum1Ptr, aPtr, aLen, bPtr, bLen, 0));

					// Compute c + d.
					Digit sum2Carry(Primitives::unevenAdd(sum2Ptr, cPtr, cLen, dPtr, dLen, 0));

					// Do the multiplies.
					// Find (a + b)(c + d).
					recursiveMul(ans + halfSize, sum1Ptr, sum1Len, sum2Ptr, sum2Len, sum2Ptr + sum2Len);

					// If there were carries, add the appropriate sums to the product. Keep the carries
					// outside the products to ensure that we don't overrun the answer buffer into any
					// working room (if we're in an intermediate calculation). Note that 3*halfSize is
					// the largest allowable amount which can be added to ans in the above multiply,
					// which is at most uLen + 1 + vLen - halfSize = (uLen + vLen) - (halfSize - 1) <=
					// uLen + vLen since halfSize is always at least 1. If we included the carries,
					// however, always, even if they were zero, we might get 3*halfSize + 2, which equals
					// uLen + vLen - (halfSize - 1) + 2, and if halfSize = 2 or halfSize = 1, we have a
					// problem, and we will get a write beyond the end of the product.
					if(sum1Carry) {
						// Add sum2 at sum1Len height.
						std::size_t offs(halfSize + sum1Len);
						Primitives::propagateAdd(ans + offs, (uLen + vLen) - offs, sum2Ptr, sum2Len);
					}

					if(sum2Carry) {
						// Add sum1 at sum2Len height.
						std::size_t offs(halfSize + sum2Len);
						Primitives::propagateAdd(ans + offs, (uLen + vLen) - offs, sum1Ptr, sum1Len);
					}

					if(sum1Carry && sum2Carry) {
						// Add a "1" at sum1Len + sum2Len height.
						std::size_t offs(halfSize + sum1Len + sum2Len);
						Primitives::propagateCarry(ans + offs, ans + offs, 1, (uLen + vLen) - offs);
					}

					// Now we can overwrite the work space used for those sums as we don't need them any
					// more.

					// Compute ac.
					recursiveMul(work, aPtr, aLen, cPtr, cLen, work + (aLen + cLen));

					// Add ac B^(2n) and subtract ac B^n.
					Primitives::propagateSub(ans + halfSize, (uLen + vLen) - halfSize, work,
						aLen + cLen);
					Primitives::propagateAdd(ans + (2 * halfSize), (uLen + vLen) - 2 * halfSize, work,
						aLen + cLen);

					// Compute bd.
					recursiveMul(work, bPtr, bLen, dPtr, dLen, work + (bLen + dLen));

					// Add bd and subtract bd B^n.
					Primitives::propagateAdd(ans, uLen + vLen, work, bLen + dLen);
					Primitives::propagateSub(ans + halfSize, (uLen + vLen) - halfSize, work,
						bLen + dLen);
				}
			}
		}
	}
}
