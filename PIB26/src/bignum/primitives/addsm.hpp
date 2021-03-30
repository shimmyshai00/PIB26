/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      addsm.hpp
 * Timestamp: Mar 23, 2021
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

#ifndef SRC_BIGNUM_PRIMITIVES_ADDSM_HPP_
#define SRC_BIGNUM_PRIMITIVES_ADDSM_HPP_

#include "../../memory/SafePtr.hpp"

#include "../defs.hpp"

#include <cstddef>

namespace SDF::Bignum::Primitives
{
	// Function:  addSmall
	// Purpose:   Add a small number to digits in a buffer.
	// Arguments: r - A pointer into the buffer to hold the result.
	//            a - A pointer into a buffer holding the first operand.
	//            smallNum - The small number to add.
	//            len - The number of digits to add.
	// Returns:   The remaining small number after the addition, plus any carry.
	unsigned int addSmall(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a,
		unsigned long smallNum, std::size_t len);
}

#endif /* SRC_BIGNUM_PRIMITIVES_ADDSM_HPP_ */
