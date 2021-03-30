/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      ucompare.cpp
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
#include "../../memory/buffers/local/StackAlloc.hpp"

#include "../primitives/assign.hpp"
#include "../primitives/compare.hpp"

namespace SDF::Bignum {
	int BigInt::ucompare(const BigInt &rhs) const {
		// Get lengths.
		std::size_t aLength(getDgsUsed());
		std::size_t bLength(rhs.getDgsUsed());

		// Handle each length case.
		if(aLength > bLength) {
			return +1; // exploit class invariant
		} else if(aLength < bLength) {
			return -1;
		} else {
			// Now we have to compare the buffers directly.
			return Primitives::compare(m_digits, rhs.m_digits, aLength);
		}
	}

	int BigInt::ucompare(unsigned int smallNum) const {
		// Get lengths.
		std::size_t aLength(getDgsUsed());
		std::size_t bLength(0);

		// Expand the small number into a bignum buffer.
		Memory::Buffers::Local::StackAlloc<Digit, DIGS_PER_SMALL> smallNumBuf;
		Memory::SafePtr<Digit> bufPtr(smallNumBuf.accessData(0));
		bLength = Primitives::assignSmall(bufPtr, smallNum, DIGS_PER_SMALL);

		// Compare.
		if(aLength > bLength) {
			return +1; // exploit class invariant
		} else if(aLength < bLength) {
			return -1;
		} else {
			// Now we have to compare the buffers directly.
			return Primitives::compare(m_digits, bufPtr, aLength);
		}
	}
}
