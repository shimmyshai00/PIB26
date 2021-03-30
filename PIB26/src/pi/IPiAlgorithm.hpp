/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      IPiAlgorithm.hpp
 * Timestamp: Mar 20, 2021
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

#ifndef SRC_PI_IPIALGORITHM_HPP_
#define SRC_PI_IPIALGORITHM_HPP_

#include "../bignum/BigFloat.hpp"

#include <memory>

namespace SDF::Pi {
	// Class:      IPiAlgorithm
	// Purpose:    Provides an interface for pi algorithms.
	// Parameters: None.
	class IPiAlgorithm {
		public:
			virtual ~IPiAlgorithm() = default;

			// Function:   computePi
			// Purpose:    Compute pi to a given number of digits.
			// Parameters: numDigits - The number of pi digits to compute.
			// Returns:    The computation result.
			virtual std::shared_ptr<Bignum::BigFloat> computePi(std::size_t numDigits) = 0;
	};
}

#endif /* SRC_PI_IPIALGORITHM_HPP_ */
