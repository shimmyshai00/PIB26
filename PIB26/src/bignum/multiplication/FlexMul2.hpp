/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      FlexMul2.hpp
 * Timestamp: Mar 22, 2021
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

#ifndef SRC_BIGNUM_MULTIPLICATION_FLEXMUL2_HPP_
#define SRC_BIGNUM_MULTIPLICATION_FLEXMUL2_HPP_

#include "../IMultiplicationStrategy.hpp"

#include <vector>

namespace SDF::Bignum::Multiplication
{
	// Class:   FlexMul2
	// Purpose: Implements a "flex" mul strategy allowing us to combine two other multiplication
	//          strategies for high performance over a wider range of possible number sizes.
	class FlexMul2 : public IMultiplicationStrategy
	{
		public:
			// Function:  FlexMul2
			// Purpose:   Construct a new 2-banger flex multiply strategy object.
			// Arguments: strategy1 - The first strategy to add.
			//            strategy2 - The second strategy to add.
			//            overrideLen - The length at which to switch from the first to the second
			//            strategy.
			FlexMul2(IMultiplicationStrategy *strategy1, IMultiplicationStrategy *strategy2,
				std::size_t overrideLen);

			void mulDigits(Memory::SafePtr<Digit> a, std::size_t aLen, Memory::SafePtr<Digit> b,
				std::size_t bLen);

			std::size_t getProductLength() const;

			void getProductDigits(Memory::SafePtr<Digit> dst, std::size_t origin, std::size_t length);
		private:
			IMultiplicationStrategy *m_strategy1;
			IMultiplicationStrategy *m_strategy2;
			std::size_t m_overrideLen;

			IMultiplicationStrategy *m_lastStrategy;
	};
}

#endif /* SRC_BIGNUM_MULTIPLICATION_FLEXMUL2_HPP_ */
