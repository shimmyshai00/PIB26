/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      ClassicalSmallMul.hpp
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

#ifndef SRC_BIGNUM_MULTIPLICATION_CLASSICALSMALLMUL_HPP_
#define SRC_BIGNUM_MULTIPLICATION_CLASSICALSMALLMUL_HPP_

#include "../IMultiplicationStrategy.hpp"

#include "../../memory/buffers/local/RAMOnly.hpp"

namespace SDF::Bignum::Multiplication
{
	// Class:   ClassicalSmallMul
	// Purpose: Implements a classical "grade school" multiplication strategy for small numbers.
	//          This is RAM-only and has O(n^2) complexity, so it is not efficient at all except for
	//          small inputs. For larger inputs, the Karatsuba (TBA), FFT (TBA), and NTT (TBA)
	//          methods should be used.
	class ClassicalSmallMul : public IMultiplicationStrategy
	{
		public:
			// Function:  ClassicalSmallMul
			// Purpose:   Construct a new strategy object with a given maximum buffer size.
			// Arguments: maxProdSize - The maximum multiplication size to allow.
			ClassicalSmallMul(std::size_t maxProdSize);

			void mulDigits(Memory::SafePtr<Digit> a, std::size_t aLen, Memory::SafePtr<Digit> b,
				std::size_t bLen);
			void squareDigits(Memory::SafePtr<Digit> a, std::size_t aLen);

			std::size_t getProductLength() const;

			void getProductDigits(Memory::SafePtr<Digit> dst, std::size_t origin, std::size_t length);
		private:
			std::size_t m_lastProductLength;
			Memory::Buffers::Local::RAMOnly<Digit> m_productBuffer;
	};
}

#endif /* SRC_BIGNUM_MULTIPLICATION_CLASSICALSMALLMUL_HPP_ */
