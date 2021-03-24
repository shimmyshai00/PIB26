/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      chudnovsky.hpp
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

#ifndef SRC_PI_BSP_CHUDNOVSKY_HPP_
#define SRC_PI_BSP_CHUDNOVSKY_HPP_

#include "../IPiAlgorithm.hpp"

#include "bsp.hpp"

#include "../../bignum/IMultiplicationStrategy.hpp"

namespace SDF::Pi::BSP {
	// Class:      Chudnovsky
	// Purpose:    Defines the Chudnovsky BSP algorithm.
	// Parameters: None.
	class Chudnovsky : public BSP {
		public:
			Chudnovsky(Bignum::IMultiplicationStrategy *multiplicationStrategy);

			std::shared_ptr<Bignum::BigFloat> computePi(std::size_t numDigits);
		protected:
			void p(Bignum::BigInt &res, unsigned int b);
			void q(Bignum::BigInt &res, unsigned int b);
			void r(Bignum::BigInt &res, unsigned int b);

			std::size_t estimatePPrec(unsigned int a, unsigned int b);
			std::size_t estimateQPrec(unsigned int a, unsigned int b);
			std::size_t estimateRPrec(unsigned int a, unsigned int b);
		private:
			// as said, this is really basic/crude right now!
			Bignum::BigInt m_A, m_B, m_C, m_smallTmp;
	};
}

#endif /* SRC_PI_BSP_CHUDNOVSKY_HPP_ */
