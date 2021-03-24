/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      IMultiplicationStrategy.hpp
 * Timestamp: Mar 6, 2021
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

#ifndef SRC_BIGNUM_IMULTIPLICATIONSTRATEGY_HPP_
#define SRC_BIGNUM_IMULTIPLICATIONSTRATEGY_HPP_

#include "defs.hpp"

#include "../memory/SafePtr.hpp"

#include <cstddef>

namespace SDF::Bignum
{
	// Class:      IMultiplicationStrategy
	// Purpose:    Defines an interface for multiplication strategies - different multiplication
	//             algorithm implementations.
	// Parameters: None.
	class IMultiplicationStrategy
	{
		public:
			virtual ~IMultiplicationStrategy() = default;

			// Function:   mulDigits
			// Purpose:    Multiply two digit buffer segments.
			// Parameters: a - The pointer to the beginning of the first digit segment.
			//             aLen - The length of the first segment in digits.
			//             b - The pointer to the beginning of the second digit segment.
			//             bLen - The length of the second segment in digits.
			// Returns:    None.
			virtual void mulDigits(Memory::SafePtr<Digit> a, std::size_t aLen,
				Memory::SafePtr<Digit> b, std::size_t bLen) = 0;

			// Function:   squareDigits
			// Purpose:    Square a digit buffer segment. Squaring can often be done more quickly than
			//             general multiplication.
			// Parameters: a - The pointer to the digit segment to square.
			//             aLen - The length of the segment in digits.
			// Returns:    None.
			virtual void squareDigits(Memory::SafePtr<Digit> a, std::size_t aLen) = 0;

			// Function:   getProductLength
			// Purpose:    Get the length of a computed product in digits.
			// Parameters: None.
			// Returns:    The length.
			virtual std::size_t getProductLength() const = 0;

			// Function:   getProductDigits
			// Purpose:    Extract the digits of a computed product.
			// Parameters: dst - Pointer into the destination buffer.
			//             origin - The index of the least-significant digit to extract.
			//             length - The number of digits to extract.
			// Returns:    None.
			virtual void getProductDigits(Memory::SafePtr<Digit> dst, std::size_t origin,
				std::size_t length) = 0;
	};
}

#endif /* SRC_BIGNUM_IMULTIPLICATIONSTRATEGY_HPP_ */
