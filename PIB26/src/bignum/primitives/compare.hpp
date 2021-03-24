/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      compare.hpp
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

#ifndef SRC_BIGNUM_PRIMITIVES_COMPARE_HPP_
#define SRC_BIGNUM_PRIMITIVES_COMPARE_HPP_

#include "../../memory/SafePtr.hpp"

#include "../defs.hpp"

#include <cstddef>

namespace SDF::Bignum::Primitives {
	// Function:  countSignifDigits
	// Purpose:   Count the significant digits in a buffer.
	// Arguments: r - A pointer into the buffer to do the counting in.
	//            len - The length of the region to count the digits in.
	// Returns:   The number of significant digits in the region starting at r and of length len.
	std::size_t countSignifDigits(Memory::SafePtr<const Digit> r, std::size_t len);

	// Function:  countSignifDigits
	// Purpose:   Count the significant digits in a small number.
	// Arguments: smallNum - The small number to count the digits in.
	// Returns:   The number of significant digits in smallNum.
	std::size_t countSignifDigits(TwoDigit smallNum);

	// Function:  testZero
	// Purpose:   Test if a region of a buffer is zero.
	// Arguments: r - A pointer into the buffer to test.
	//            len - The number of digits to test.
	// Returns:   Whether the region is zero.
	bool testZero(Memory::SafePtr<const Digit> r, std::size_t len);

	// Function:  revTestZero
	// Purpose:   Test if a region of a buffer is zero, but with pointer to the MSD passed instead.
	// Arguments: r - A pointer into the buffer to test, at *one past* the region's MSD.
	//            len - The number of digits to test.
	// Returns:   Whether the region is zero.
	bool revTestZero(Memory::SafePtr<const Digit> r, std::size_t len);

	// Function:  compare
	// Purpose:   Compare two buffer regions.
	// Arguments: a - A pointer to the first operand.
	//            b - A pointer to the second operand.
	//            len - The number of digits to compare.
	// Returns:   +1 iff a > b
	//             0 iff a == b
	//            -1 iff a < b.
	int compare(Memory::SafePtr<const Digit> a, Memory::SafePtr<const Digit> b, std::size_t len);

	// Function:  revCompare
	// Purpose:   Compare two buffer regions, but with pointers to the MSDs passed instead.
	// Arguments: a - A pointer to *one past* the first operand's MSD.
	//            b - A pointer to *one past* the second operand's MSD.
	//            len - The number of digits to compare.
	// Returns:   +1 iff a > b
	//             0 iff a == b
	//            -1 iff a < b.
	int revCompare(Memory::SafePtr<const Digit> a, Memory::SafePtr<const Digit> b, std::size_t len);
}

#endif /* SRC_BIGNUM_PRIMITIVES_COMPARE_HPP_ */
