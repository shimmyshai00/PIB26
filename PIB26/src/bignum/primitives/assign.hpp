/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      assign.hpp
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

#ifndef SRC_BIGNUM_PRIMITIVES_ASSIGN_HPP_
#define SRC_BIGNUM_PRIMITIVES_ASSIGN_HPP_

#include "../../memory/SafePtr.hpp"

#include "../defs.hpp"

#include <cstddef>

namespace SDF::Bignum::Primitives
{
	// Function:  zeroize
	// Purpose:   Zeroize digits in a buffer.
	// Arguments: r - A pointer into the buffer to zeroize.
	//            len - The number of digits to zeroize.
	// Returns:   None.
	void zeroize(Memory::SafePtr<Digit> r, std::size_t len);

	// Function:  nineize
	// Purpose:   Nineize (fill with "nines", i.e. BASE-1) digits in a buffer.
	// Arguments: r - A pointer into the buffer to nineize.
	//            len - The number of digits to nineize.
	// Returns:   None.
	void nineize(Memory::SafePtr<Digit> r, std::size_t len);

	// Function:  copy
	// Purpose:   Copy digits from one buffer into another.
	// Arguments: dst - A pointer into the destination buffer.
	//            src - A pointer into the source buffer.
	//            len - The number of digits to copy.
	// Returns:   None.
	void copy(Memory::SafePtr<Digit> dst, Memory::SafePtr<const Digit> src, std::size_t len);

	// Function:  assignSmall
	// Purpose:   Assign a small number to a buffer.
	// Arguments: dst - A pointer into the destination buffer.
	//            smallNum - The small number to assign.
	//            maxLen - The maximum allowable length of the result.
	// Returns:   The number of digits assigned.
	std::size_t assignSmall(Memory::SafePtr<Digit> dst, unsigned int smallNum, std::size_t maxLen);
}

#endif /* SRC_BIGNUM_PRIMITIVES_ASSIGN_HPP_ */
