/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      SmallKaratsuba.hpp
 * Timestamp: Mar 21, 2021
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

#ifndef SRC_BIGNUM_MULTIPLICATION_SMALLKARATSUBA_HPP_
#define SRC_BIGNUM_MULTIPLICATION_SMALLKARATSUBA_HPP_

#include "../IMultiplicationStrategy.hpp"

#include "../../memory/buffers/local/RAMOnly.hpp"

namespace SDF::Bignum::Multiplication
{
	// Class:      SmallKaratsuba
	// Purpose:    Implements a Karatsuba O(n^log_2(3)) multiplication for smaller numbers, but
	//             still larger than those for ClassicalSmallMul (the crossover point requires some
	//             calibration), though not as large as those where the FFT (code TBA) would be used.
	// Parameters: None.
	class SmallKaratsuba : public IMultiplicationStrategy
	{
		public:
			// Function:  SmallKaratsuba
			// Purpose:   Construct a new instance with a given maximum buffer size.
			// Arguments: maxProdSize - The maximum multiplication size to allow.
			SmallKaratsuba(std::size_t maxProdSize);

			void mulDigits(Memory::SafePtr<Digit> a, std::size_t aLen, Memory::SafePtr<Digit> b,
				std::size_t bLen);
			void squareDigits(Memory::SafePtr<Digit> a, std::size_t aLen);

			std::size_t getProductLength() const;

			void getProductDigits(Memory::SafePtr<Digit> dst, std::size_t origin, std::size_t length);
		private:
			std::size_t m_lastProductLength;
			Memory::Buffers::Local::RAMOnly<Digit> m_productBuffer;
			Memory::Buffers::Local::RAMOnly<Digit> m_workBuffer;

			void recursiveMul(Memory::SafePtr<Digit> ans, Memory::SafePtr<Digit> u, std::size_t uLen,
				Memory::SafePtr<Digit> v, std::size_t vLen, Memory::SafePtr<Digit> work);
	};
}

#endif /* SRC_BIGNUM_MULTIPLICATION_SMALLKARATSUBA_HPP_ */
