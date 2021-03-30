/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      usub.cpp
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

#include "../primitives/assign.hpp"
#include "../primitives/compare.hpp"
#include "../primitives/sub.hpp"
#include "../primitives/shift.hpp"

namespace SDF::Bignum
{
	void BigFloat::usub(const BigFloat &num1, const BigFloat &num2)
	{
		// This is very much like the addition, but since it doesn't commute, we need to treat
		// each exponent case explicitly.
		Digit borrow(0);
		if (num1.m_exp >= num2.m_exp) {
			std::size_t ediff(num1.m_exp - num2.m_exp);
			if (ediff >= m_totalLen) {
				// num2 completely gone.
				uassign(num1);
				return;
			}

			m_exp = num1.m_exp; // set provisional exponent.

			// Figure the placement of the MSD of num1 and num2 within this and crop. */
			std::size_t num1MsdPos(m_totalLen); // one past
			std::size_t num2MsdPos(m_totalLen - ediff); // one past

			std::size_t num1Length(num1.m_totalLen);
			std::size_t num2Length(num2.m_totalLen);

			Memory::SafePtr<Digit> rPtr(m_digits);
			Memory::SafePtr<Digit> num1Ptr(num1.m_digits);
			Memory::SafePtr<Digit> num2Ptr(num2.m_digits);

			if (num1MsdPos < num1Length) {
				std::size_t crop(num1Length - num1MsdPos);
				num1Length -= crop;
				num1Ptr += crop;
			}

			if (num2MsdPos < num2Length) {
				std::size_t crop(num2Length - num2MsdPos);
				num2Length -= crop;
				num2Ptr += crop;
			}

			// Handle the length cases.
			if (ediff + num2Length <= num1Length) {
				// Case: RRRRRRRRRRR
				//       AAAAAAAAA
				//          BBB
				std::size_t num1LowerOverhang(num1Length - (ediff + num2Length));
				std::size_t num12Overlap(num2Length);
				std::size_t num1UpperOverhang(ediff);

				rPtr += (m_totalLen - num1Length);
				Primitives::copy(rPtr, num1Ptr, num1LowerOverhang);
				rPtr += num1LowerOverhang;
				num1Ptr += num1LowerOverhang;
				borrow = Primitives::sub(rPtr, num1Ptr, num2Ptr, num12Overlap, borrow);
				rPtr += num12Overlap;
				num1Ptr += num12Overlap;
				borrow = Primitives::propagateBorrow(rPtr, num1Ptr, borrow, num1UpperOverhang);
			} else {
				if (ediff < num1Length) {
					// Case: RRRRRRRRRRR
					//       AAAAAA
					//           BBBBB
					std::size_t num2Overhang(ediff + num2Length - num1Length);
					std::size_t num12Overlap(num2Length - num2Overhang);
					std::size_t num1Overhang(ediff);

					rPtr += (num2MsdPos - num2Length);
					borrow = Primitives::neg(rPtr, num2Ptr, borrow, num2Overhang);
					rPtr += num2Overhang;
					num2Ptr += num2Overhang;
					borrow = Primitives::sub(rPtr, num1Ptr, num2Ptr, num12Overlap, borrow);
					rPtr += num12Overlap;
					num1Ptr += num12Overlap;
					borrow = Primitives::propagateBorrow(rPtr, num1Ptr, borrow, num1Overhang);
				} else {
					// Case: RRRRRRRRRRR
					//       AAAA
					//              BBBB
					std::size_t num12Gap(ediff - num1Length);
					borrow = Primitives::neg(rPtr, num2Ptr, borrow, num2Length);
					rPtr += num2Length;
					if (!borrow) {
						Primitives::zeroize(rPtr, num12Gap);
					} else {
						Primitives::nineize(rPtr, num12Gap);
					}
					rPtr += num12Gap;
					borrow = Primitives::propagateBorrow(rPtr, num1Ptr, borrow, num1Length);
				}
			}
		} else {
			// Same as above, but flipped around.
			std::size_t ediff(num2.m_exp - num1.m_exp);
			if (ediff >= m_totalLen) {
				// num1 completely gone.
				uassign(num2);
				m_sign = SIGN_NEGATIVE;
				return;
			}

			m_exp = num2.m_exp; // set provisional exponent.

			// Figure the placement of the MSD of num1 and num2 within this and crop. */
			std::size_t num1MsdPos(m_totalLen - ediff); // one past
			std::size_t num2MsdPos(m_totalLen); // one past

			std::size_t num1Length(num1.m_totalLen);
			std::size_t num2Length(num2.m_totalLen);

			Memory::SafePtr<Digit> rPtr(m_digits);
			Memory::SafePtr<Digit> num1Ptr(num1.m_digits);
			Memory::SafePtr<Digit> num2Ptr(num2.m_digits);

			if (num1MsdPos < num1Length) {
				std::size_t crop(num1Length - num1MsdPos);
				num1Length -= crop;
				num1Ptr += crop;
			}

			if (num2MsdPos < num2Length) {
				std::size_t crop(num2Length - num2MsdPos);
				num2Length -= crop;
				num2Ptr += crop;
			}

			// Handle the length cases.
			if (ediff + num1Length <= num2Length) {
				// Case: RRRRRRRRRRR
				//          AAA
				//       BBBBBBBBB
				std::size_t num2LowerOverhang(num2Length - (ediff + num1Length));
				std::size_t num12Overlap(num1Length);
				std::size_t num2UpperOverhang(ediff);

				rPtr += (m_totalLen - num2Length);
				borrow = Primitives::neg(rPtr, num2Ptr, borrow, num2LowerOverhang);
				rPtr += num2LowerOverhang;
				num2Ptr += num2LowerOverhang;
				borrow = Primitives::sub(rPtr, num1Ptr, num2Ptr, num12Overlap, borrow);
				rPtr += num12Overlap;
				num2Ptr += num12Overlap;
				borrow = Primitives::neg(rPtr, num2Ptr, borrow, num2UpperOverhang);
			} else {
				if (ediff < num2Length) {
					// Case: RRRRRRRRRRR
					//           AAAAA
					//       BBBBBB
					std::size_t num1Overhang(ediff + num1Length - num2Length);
					std::size_t num12Overlap(num1Length - num1Overhang);
					std::size_t num2Overhang(ediff);

					rPtr += (num1MsdPos - num1Length);
					Primitives::copy(rPtr, num1Ptr, num1Overhang);
					rPtr += num1Overhang;
					num1Ptr += num1Overhang;
					borrow = Primitives::sub(rPtr, num1Ptr, num2Ptr, num12Overlap, borrow);
					rPtr += num12Overlap;
					num2Ptr += num12Overlap;
					borrow = Primitives::neg(rPtr, num2Ptr, borrow, num2Overhang);
				} else {
					// Case: RRRRRRRRRRR
					//              AAAA
					//       BBBB
					std::size_t num12Gap(ediff - num2Length);
					Primitives::copy(rPtr, num1Ptr, num1Length);
					rPtr += num1Length;
					Primitives::zeroize(rPtr, num12Gap);
					rPtr += num12Gap;
					borrow = Primitives::neg(rPtr, num2Ptr, borrow, num2Length);
				}
			}
		}

		// Handle borrow.
		if (borrow) {
			// A borrow out means the result is negative. Negate.
			Primitives::neg(m_digits, m_digits, 0, m_totalLen);
			m_sign = SIGN_NEGATIVE;
		} else {
			m_sign = SIGN_POSITIVE;
		}

		// Handle cancellation.
		std::size_t signif(Primitives::countSignifDigits(m_digits, m_totalLen));
		if (signif < m_totalLen) {
			if (signif == 0) {
				// complete cancellation.
				m_sign = SIGN_POSITIVE;
				m_exp = 0;
			} else {
				Primitives::lshift(m_digits, m_totalLen - signif, signif);
				m_exp -= (m_totalLen - signif);
			}
		}
	}

	void BigFloat::usubIp(const BigFloat &num)
	{
		Digit borrow(0);
		if (m_exp >= num.m_exp) {
			// num is shifted right. Crop if necessary.
			std::size_t ediff(m_exp - num.m_exp);
			if (ediff >= m_totalLen) {
				// num completely gone.
				return;
			}

			// Figure the placement of the MSD of num within this and crop. */
			std::size_t numMsdPos(m_totalLen - ediff); // one past
			std::size_t numLength(num.m_totalLen);

			Memory::SafePtr<Digit> rPtr(m_digits);
			Memory::SafePtr<Digit> numPtr(num.m_digits);

			if (numMsdPos < numLength) {
				std::size_t crop(numLength - numMsdPos);
				numLength -= crop;
				numPtr += crop;
			}

			// Now there's only one length case.
			// Case 1: RRRRRRRRRRR
			//            NNNNN
			std::size_t rLowerOverhang(m_totalLen - (ediff + numLength));
			std::size_t rnOverlap(numLength);
			std::size_t rUpperOverhang(ediff);
			rPtr += rLowerOverhang;
			borrow = Primitives::sub(rPtr, rPtr, numPtr, rnOverlap, borrow);
			rPtr += rnOverlap;
			borrow = Primitives::propagateBorrow(rPtr, rPtr, borrow, rUpperOverhang);
		} else {
			// this is shifted right. We have to now shift explicitly.
			std::size_t ediff(num.m_exp - m_exp);
			if (ediff >= m_totalLen) {
				// this completely gone.
				uassign(num);
				m_sign = SIGN_NEGATIVE;
				return;
			}

			m_exp = num.m_exp; // set provisional exponent

			// Shift this by the given amount.
			Primitives::rshift(m_digits, ediff, m_totalLen);

			// Crop num if necessary. Its MSD is always at m_numDigits.
			std::size_t numLength(num.m_totalLen);

			Memory::SafePtr<Digit> rPtr(m_digits);
			Memory::SafePtr<Digit> numPtr(num.m_digits);

			if (m_totalLen < numLength) {
				std::size_t crop(numLength - m_totalLen);
				numLength -= crop;
				numPtr += crop;
			}

			// Now there's only one length case.
			// Case 1: RRRRRRRRRRR (shifted)
			//         NNNNN
			std::size_t rOverhang(m_totalLen - numLength);
			std::size_t rnOverlap(numLength);
			rPtr += rOverhang;
			borrow = Primitives::sub(rPtr, rPtr, numPtr, rnOverlap, borrow);
		}

		// Handle borrow.
		if (borrow) {
			// A borrow out means the result is negative. Negate.
			Primitives::neg(m_digits, m_digits, 0, m_totalLen);
			m_sign = SIGN_NEGATIVE;
		} else {
			m_sign = SIGN_POSITIVE;
		}

		// Handle cancellation.
		std::size_t signif(Primitives::countSignifDigits(m_digits, m_totalLen));
		if (signif < m_totalLen) {
			if (signif == 0) {
				// complete cancellation.
				m_sign = SIGN_POSITIVE;
				m_exp = 0;
			} else {
				Primitives::lshift(m_digits, m_totalLen - signif, signif);
				m_exp -= (m_totalLen - signif);
			}
		}
	}
}
