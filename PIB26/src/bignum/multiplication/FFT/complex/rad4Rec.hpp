/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      rad4Rec.hpp
 * Timestamp: Mar 31, 2021
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
#ifndef SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_RAD4REC_HPP_
#define SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_RAD4REC_HPP_

#include "ComplexFft.hpp"

#include "../../../../memory/SafePtr.hpp"

#include "Cplex.hpp"

#include "rad2Itr.hpp"

#include <cstddef>

namespace SDF::Bignum::Multiplication::Fft::Complex {
	// Class:      rad4Rec
	// Purpose:    Wraps the radix-4 recursive complex FFT.
	// Parameters: None.
	class rad4Rec : public ComplexFft {
		public:
			rad4Rec(Memory::SafePtr<Cplex> omegaTable, std::size_t omegaSize);

			std::size_t getMaxFftSize() const;
			std::size_t getNearestSafeLengthTo(std::size_t length) const;

			void doFwdTransform(Memory::SafePtr<Cplex> data, std::size_t len);
			void doRevTransform(Memory::SafePtr<Cplex> data, std::size_t len);
		private:
			Memory::SafePtr<Cplex> m_omegaTable;
			std::size_t m_omegaSize;

			std::size_t m_iterativeFftThreshold;
			rad2Itr m_iterativeFft;
	};
}

#endif /* SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_RAD4REC_HPP_ */
