/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      sub.hpp
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

#ifndef SRC_BIGNUM_PRIMITIVES_SUB_HPP_
#define SRC_BIGNUM_PRIMITIVES_SUB_HPP_

#include "../../memory/SafePtr.hpp"

#include "../defs.hpp"

#include <cstddef>

namespace SDF::Bignum::Primitives
{
	// Function:  sub
	// Purpose:   Sub digits in one buffer from those in another.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            a - A pointer into a buffer holding the first operand.
	//            b - A pointer into a buffer holding the second operand.
	//            len - The number of digits to sub.
	//            borrow - The incoming borrow flag.
	// Returns:   The outgoing borrow flag after the subtraction.
	Digit sub(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a,
		Memory::SafePtr<const Digit> b, std::size_t len, Digit borrow);

	// Function:  propagateBorrow
	// Purpose:   Propagate a borrow through a digit sequence.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            a - A pointer into the buffer holding toe digits to propagate through.
	//            borrow - The borrow to propagate.
	//            len - The number of digits to propagate through.
	// Returns:   The outgoing borrow flag after the propagation.
	Digit propagateBorrow(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a, Digit borrow,
		std::size_t len);

	// Function:  neg
	// Purpose:   Negate (complement) a digit segment.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            a - A pointer into the buffer holding toe digits to negate.
	//            borrow - The borrow to propagate.
	//            len - The number of digits to negate.
	// Returns:   The outgoing borrow flag after the negation.
	Digit neg(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a, Digit borrow,
		std::size_t len);

	// Function:  propagateSub
	// Purpose:   Effectively an in-place version of the above sub, that automatically propagates a
	//            carry up its first argument after the sub ends.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            rLen - The number of digits from "r" to sub.
	//            a - A pointer into a buffer holding the second operand.
	//            aLen - The number of digits from "a" to sub.
	// Returns:   None.
	void propagateSub(Memory::SafePtr<Digit> r, std::size_t rLen, Memory::SafePtr<const Digit> a,
		std::size_t aLen);
}

#endif /* SRC_BIGNUM_PRIMITIVES_SUB_HPP_ */
