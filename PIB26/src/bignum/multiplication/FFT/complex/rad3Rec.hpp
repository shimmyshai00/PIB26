/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      rad3Rec.hpp
 * Timestamp: Mar 25, 2021
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
#ifndef SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_RAD3REC_HPP_
#define SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_RAD3REC_HPP_

#include "ComplexFft.hpp"

#include "../../../../memory/SafePtr.hpp"

#include "Cplex.hpp"

#include "rad2Rec.hpp"

#include <cstddef>

namespace SDF::Bignum::Multiplication::Fft::Complex {
	// Class:      rad3Rec
	// Purpose:    Wraps the radix-3 recursive complex FFT.
	// Parameters: None.
	class rad3Rec : public ComplexFft {
		public:
			rad3Rec();

			std::size_t getNearestSafeLengthTo(std::size_t length) const;

			void doFwdTransform(Memory::SafePtr<Cplex> data, std::size_t len);
			void doRevTransform(Memory::SafePtr<Cplex> data, std::size_t len);
		private:
			rad2Rec m_rad2Fft;
	};
}

#endif /* SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_RAD3REC_HPP_ */
