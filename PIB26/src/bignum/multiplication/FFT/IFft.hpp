/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      IFft.hpp
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

#ifndef SRC_BIGNUM_MULTIPLICATION_FFT_IFFT_HPP_
#define SRC_BIGNUM_MULTIPLICATION_FFT_IFFT_HPP_

#include "../../../memory/SafePtr.hpp"

#include <cstddef>

namespace SDF::Bignum::Multiplication::Fft {
	// Class:      IFft
	// Purpose:    Defines an interface for FFT algorithms.
	// Parameters: ElementT - The type of ring element to use.
	template<class ElementT>
	class IFft {
		public:
			virtual ~IFft() = default;

			// Function:   getMaxNumLengthAtBase
			// Purpose:    Get the largest allowed number length when packing numbers in a given base.
			// Parameters: base - The base to use.
			// Returns:    The maximum length of a number in the given base that won't overflow the
			//             FFT during multiplication.
			virtual std::size_t getMaxNumLengthAtBase(long base) const = 0;

			// Function:   getMaxFftSize
			// Purpose:    Gets the maximum allowed FFT size.
			// Parameters: None.
			// Returns:    The largest permissible FFT size.
			virtual std::size_t getMaxFftSize() const = 0;

			// Function:   getNearestSafeLengthTo
			// Purpose:    Gets the smallest "safe" (that is, okay to use) length sufficient to
			//             transform a given length of data. Data should be padded to this length
			//             prior to transforming.
			// Parameters: length - The generic length to test.
			// Returns:    The smallest safe length just larger than length.
			virtual std::size_t getNearestSafeLengthTo(std::size_t length) const = 0;

			// Function:   doFwdTransform/doRevTransform
			// Purpose:    Perform the forward/reverse-direction FFT.
			// Parameters: data - The data to transform.
			//             len - The length to transform. Note: not all FFT algorithms support all
			//                   lengths! Use the above method to obtain the smallest "safe" length
			//                   sufficient for transforming a given amount of data.
			virtual void doFwdTransform(Memory::SafePtr<ElementT> data, std::size_t len) = 0;
			virtual void doRevTransform(Memory::SafePtr<ElementT> data, std::size_t len) = 0;
	};
}

#endif /* SRC_BIGNUM_MULTIPLICATION_FFT_IFFT_HPP_ */
