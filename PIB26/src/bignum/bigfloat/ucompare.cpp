/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      ucompare.cpp
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

#include "../../memory/buffers/local/StackAlloc.hpp"

#include "../primitives/assign.hpp"
#include "../primitives/compare.hpp"

namespace SDF::Bignum
{
	int BigFloat::ucompare(const BigFloat &rhs) const
	{
		// Since we assume the BigFloats are normalized, we can compare the exponents first.
		if (m_exp > rhs.m_exp) {
			return +1;
		} else if (m_exp < rhs.m_exp) {
			return -1;
		} else {
			// Now we need to compare the digits. There are two length cases to consider.
			if (m_totalLen >= rhs.m_totalLen) {
				// Case: AAAAAAAAAAAAAA
				//       BBBBBBB
				std::size_t aOverhang(m_totalLen - rhs.m_totalLen);
				std::size_t abOverlap(rhs.m_totalLen);
				int cmpResult(Primitives::compare(m_digits + aOverhang, rhs.m_digits, abOverlap));

				if (cmpResult != 0) {
					return cmpResult;
				} else {
					// The overhanging bit of lhs is the final decider.
					if (!Primitives::testZero(m_digits, aOverhang)) {
						return +1;
					} else {
						return 0;
					}
				}
			} else {
				// Case: AAAAAAA
				//       BBBBBBBBBBBBBB
				std::size_t bOverhang(rhs.m_totalLen - m_totalLen);
				std::size_t abOverlap(m_totalLen);
				int cmpResult(Primitives::compare(m_digits, rhs.m_digits + bOverhang, abOverlap));

				if (cmpResult != 0) {
					return cmpResult;
				} else {
					// The overhanging bit of rhs is the final decider.
					if (!Primitives::testZero(rhs.m_digits, bOverhang)) {
						return -1;
					} else {
						return 0;
					}
				}
			}
		}
	}

	int BigFloat::ucompare(unsigned int smallNum) const
	{
		// Similar to uassign, first expand the small number in a separate stack buffer so we can
		// compare it.
		Memory::Buffers::Local::StackAlloc<Digit, DIGS_PER_SMALL> localBuffer;
		Memory::SafePtr<Digit> localBufPtr(localBuffer.accessData(0));

		std::size_t nRhsDigits(Primitives::assignSmall(localBufPtr, smallNum, DIGS_PER_SMALL));
		std::ptrdiff_t rhsExp((nRhsDigits != 0) ? nRhsDigits - 1 : 0);

		if (m_exp > rhsExp) {
			return +1;
		} else if (m_exp < rhsExp) {
			return -1;
		} else {
			if (m_totalLen >= nRhsDigits) {
				// Case: AAAAAAAAAAAAAA
				//       BBBBBBB
				std::size_t aOverhang(m_totalLen - nRhsDigits);
				std::size_t abOverlap(nRhsDigits);
				int cmpResult(Primitives::compare(m_digits + aOverhang, localBufPtr, abOverlap));

				if (cmpResult != 0) {
					return cmpResult;
				} else {
					// The overhanging bit of lhs is the final decider.
					if (!Primitives::testZero(m_digits, aOverhang)) {
						return +1;
					} else {
						return 0;
					}
				}
			} else {
				// Case: AAAAAAA
				//       BBBBBBBBBBBBBB
				std::size_t bOverhang(nRhsDigits - m_totalLen);
				std::size_t abOverlap(m_totalLen);
				int cmpResult(Primitives::compare(m_digits, localBufPtr + bOverhang, abOverlap));

				if (cmpResult != 0) {
					return cmpResult;
				} else {
					// The overhanging bit of rhs is the final decider.
					if (!Primitives::testZero(localBufPtr, bOverhang)) {
						return -1;
					} else {
						return 0;
					}
				}
			}
		}
	}
}
