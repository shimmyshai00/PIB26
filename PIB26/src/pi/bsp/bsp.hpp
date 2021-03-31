/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      bsp.hpp
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

#ifndef SRC_PI_BSP_BSP_HPP_
#define SRC_PI_BSP_BSP_HPP_

#include "../IPiAlgorithm.hpp"

#include "../../memory/ILocalBuffer.hpp"

#include "../../bignum/BigFloat.hpp"
#include "../../bignum/BigInt.hpp"

#include "../../bignum/IMultiplicationStrategy.hpp"

#include "../../util/ITicker.hpp"

#include <memory>
#include <cstddef>

namespace SDF::Pi::BSP
{
	// Struct:  BSPOutput
	// Purpose: Organizes the output from the BSP computation.
	struct BSPOutput
	{
			std::shared_ptr<Bignum::BigFloat> P;
			std::shared_ptr<Bignum::BigFloat> Q;
			std::shared_ptr<Bignum::BigFloat> R;
	};

	// Class:   BSP
	// Purpose: Defines a set of methods for performing a pi computation involving the binary
	//          splitting (BSP) summation of a hypergeometric series, such as those used to
	//          describe pi. This BSP method is the usual 3-factor A/P/Q (or P/Q/R) one.
	//          One overloads the relevant virtual methods to provide computation for a
	//          specific hypergeometric formula.
	class BSP : public IPiAlgorithm
	{
		public:
			BSP(Bignum::IMultiplicationStrategy *multiplicationStrategy);
			virtual ~BSP()
			{
			}
		protected:
			Bignum::IMultiplicationStrategy *m_multiplicationStrategy;

			// Function:  compute
			// Purpose:   Performs the BSP computation.
			// Arguments: a - the lower bound for the BSP computation.
			//            b - the upper bound for the BSP computation.
			//            prec - the precision to compute to.
			// Returns:   A structure containing the computation result variables.
			BSPOutput compute(unsigned int a, unsigned int b, std::size_t prec);

			// Function:   p/q/r
			// Purpose:    Compute the recursion base cases for the different variables in the BSP
			//             process.
			// Parameters: res - The BigInt to hold the base case result.
			//             n - The series point at which the base case is computed.
			virtual void p(Bignum::BigInt &res, unsigned int b) = 0;
			virtual void q(Bignum::BigInt &res, unsigned int b) = 0;
			virtual void r(Bignum::BigInt &res, unsigned int b) = 0;

			// Function:   estimatePPrec
			// Purpose:    Estimate the amount of precision required for storing the BSP P-variable at
			//             a given stage. This should implement a heuristic that should overestimate
			//             (but ideally, not too much!) the size of each parameter. This overestimation
			//             is very important - the code may not give the right answer if it
			//             underestimates, as some bignums may start to overwrite each other in
			//             unwanted ways because they're all built on the same buffers.
			// Parameters: a - the lower bound of the BSP computation to estimate the size of
			//             b - the upper bound of the BSP computation to estimate the size of
			virtual std::size_t estimatePPrec(unsigned int a, unsigned int b) = 0;

			// Same for Q, R variables
			virtual std::size_t estimateQPrec(unsigned int a, unsigned int b) = 0;
			virtual std::size_t estimateRPrec(unsigned int a, unsigned int b) = 0;
		private:
			// Small result output.
			struct SmallOutput
			{
					std::unique_ptr<Bignum::BigInt> P;
					std::unique_ptr<Bignum::BigInt> Q;
					std::unique_ptr<Bignum::BigInt> R;
			};

			// Buffers for the intermediate results.
			std::unique_ptr<Memory::ILocalBuffer<Bignum::Digit>> m_pBuffer;
			std::unique_ptr<Memory::ILocalBuffer<Bignum::Digit>> m_qBuffer;
			std::unique_ptr<Memory::ILocalBuffer<Bignum::Digit>> m_rBuffer;
			std::unique_ptr<Memory::ILocalBuffer<Bignum::Digit>> m_tmpBuffer;

			std::unique_ptr<Bignum::BigInt> m_tmpBigInt;
			std::unique_ptr<Bignum::BigFloat> m_tmpBigFloat;

			// Performs the binary splitting in full integer precision with operands built on the
			// buffers just given above.
			SmallOutput smallCompute(Memory::SafePtr<Bignum::Digit> pBufPtr,
				Memory::SafePtr<Bignum::Digit> qBufPtr, Memory::SafePtr<Bignum::Digit> rBufPtr,
				unsigned int a, unsigned int b, Util::ITicker *ticker);

			// This "giant sum" routine performs the final few series passes in floating point at full
			// precision using classical summation (i.e. breaking the series up linearly). This is
			// needed because the BSP coefficients P, Q, and R grow superlinearly in terms of their
			// number of digits and this saves some memory (though it costs a little speed).
			void giantSum(Bignum::BigFloat &P, Bignum::BigFloat &Q, Bignum::BigFloat &R,
				unsigned int a, unsigned int b, int numSteps);
	};
}

#endif /* SRC_PI_BSP_BSP_HPP_ */
