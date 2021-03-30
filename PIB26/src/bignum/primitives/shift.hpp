/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      shift.hpp
 * Timestamp: Mar 17, 2021
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

#ifndef SRC_BIGNUM_PRIMITIVES_SHIFT_HPP_
#define SRC_BIGNUM_PRIMITIVES_SHIFT_HPP_

#include "../../memory/SafePtr.hpp"

#include "../defs.hpp"

#include <cstddef>

namespace SDF::Bignum::Primitives {
	// Function:  lshift
	// Purpose:   Physically shift the digits in a buffer left by a certain amount.
	// Arguments: r - A pointer into the buffer to shift.
	//            shift - The amount to shift by.
	//            len - The length of the buffer to shift *before* shifting. The buffer must be able
	//                  to hold len + shift digits.
	// Returns:   None.
	void lshift(Memory::SafePtr<Digit> r, std::size_t shift, std::size_t len);

	// Function:  rshift
	// Purpose:   Physically shift the digits in a buffer right by a certain amount.
	// Arguments: r - A pointer into the buffer to shift.
	//            shift - The amount to shift by.
	//            len - The length of the buffer to shift.
	// Returns:   None.
	void rshift(Memory::SafePtr<Digit> r, std::size_t shift, std::size_t len);
}

#endif /* SRC_BIGNUM_PRIMITIVES_SHIFT_HPP_ */
