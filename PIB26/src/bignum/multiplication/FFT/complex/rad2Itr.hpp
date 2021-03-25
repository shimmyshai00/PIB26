/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      rad2Itr.hpp
 * Timestamp: Mar 24, 2021
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
#ifndef SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_RAD2ITR_HPP_
#define SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_RAD2ITR_HPP_

#include "ComplexFft.hpp"

#include "../../../../memory/SafePtr.hpp"

#include "Cplex.hpp"

#include <cstddef>
#include <vector>

namespace SDF::Bignum::Multiplication::Fft::Complex {
	// Class:      rad2Itr
	// Purpose:    Wraps the radix-2 iterative complex FFT. This one uses a precomputed
	//             root table, so takes extra memory.
	// Parameters: None.
	class rad2Itr : public ComplexFft {
		public:
			// NOTE: The FFT size passed to this MUST be a power of 2!
			rad2Itr(std::size_t maxFftSize);

			std::size_t getMaxFftSize() const;
			std::size_t getNearestSafeLengthTo(std::size_t length) const;

			void doFwdTransform(Memory::SafePtr<Cplex> data, std::size_t len);
			void doRevTransform(Memory::SafePtr<Cplex> data, std::size_t len);
		private:
			std::size_t m_maxFftSize;

			std::vector<Cplex> m_omegaTable;
	};
}

#endif /* SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_RAD2ITR_HPP_ */
