/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      add.hpp
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

#ifndef SRC_BIGNUM_PRIMITIVES_ADD_HPP_
#define SRC_BIGNUM_PRIMITIVES_ADD_HPP_

#include "../../memory/SafePtr.hpp"

#include "../defs.hpp"

#include <cstddef>

namespace SDF::Bignum::Primitives
{
	// Function:  add
	// Purpose:   Add digits from two buffers together.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            a - A pointer into a buffer holding the first operand.
	//            b - A pointer into a buffer holding the second operand.
	//            len - The number of digits to add.
	//            carry - The incoming carry flag.
	// Returns:   The outgoing carry flag after the addition.
	Digit add(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a,
		Memory::SafePtr<const Digit> b, std::size_t len, Digit carry);

	// Function:  propagateCarry
	// Purpose:   Propagate a carry through a digit sequence.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            a - A pointer into the buffer holding toe digits to propagate through.
	//            carry - The carry to propagate.
	//            len - The number of digits to propagate through.
	// Returns:   The outgoing carry flag after the propagation.
	Digit propagateCarry(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a, Digit carry,
		std::size_t len);

	// Function:  unevenAdd
	// Purpose:   Add digits from two buffers together, with strings of digits having different
	//            lengths. May replace the above two routines altogether.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            a - A pointer into a buffer holding the first operand.
	//            aLen - The number of digits from "a" to add.
	//            b - A pointer into a buffer holding the second operand.
	//            bLen - The number of digits from "b" to add.
	//            carry - The incoming carry flag.
	// Returns:   The outgoing carry flag after the addition.
	Digit unevenAdd(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a, std::size_t aLen,
		Memory::SafePtr<const Digit> b, std::size_t bLen, Digit carry);

	// Function:  propagateAdd
	// Purpose:   Effectively an in-place version of the above add, that automatically propagates a
	//            carry up its first argument after the addition ends.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            rLen - The number of digits from "r" to add.
	//            a - A pointer into a buffer holding the second operand.
	//            aLen - The number of digits from "a" to add.
	// Returns:   None.
	void propagateAdd(Memory::SafePtr<Digit> r, std::size_t rLen, Memory::SafePtr<const Digit> a,
		std::size_t aLen);
}

#endif /* SRC_BIGNUM_PRIMITIVES_ADD_HPP_ */
