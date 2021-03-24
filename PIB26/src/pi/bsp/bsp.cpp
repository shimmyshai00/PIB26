/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      bsp.cpp
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

#include "bsp.hpp"

#include "../../memory/buffers/local/RAMOnly.hpp"

#include <iostream>

namespace SDF::Pi::BSP
{
	using namespace Bignum;

	BSP::BSP(Bignum::IMultiplicationStrategy *multiplicationStrategy)
		: m_multiplicationStrategy(multiplicationStrategy)
	{
	}

	BSPOutput BSP::compute(unsigned int a, unsigned int b, std::size_t prec)
	{
		BSPOutput rv;

		// First, figure out how many steps we need, by figuring out the smallest number of steps
		// that makes the biggest coefficients just smaller than half the full precision.
		std::size_t numSteps(0);
		std::size_t largestSize(0);
		do {
			++numSteps;

			// The rightmost part of the series is assumed the largest.
			std::size_t testA(a + ((b - a) * (numSteps - 1) / numSteps));
			std::size_t testB(b);

			largestSize = estimatePPrec(testA, testB);
			largestSize = std::max(largestSize, estimateQPrec(testA, testB));
			largestSize = std::max(largestSize, estimateRPrec(testA, testB));
		} while (largestSize > prec);

		// Now that we have this number of steps, allocate suitable work buffers.
		// This is still not fully efficient - takes about 2.5 full-size variables (FSVs).
		m_pBuffer = std::make_unique<Memory::Buffers::Local::RAMOnly<Bignum::Digit>>(largestSize);
		m_qBuffer = std::make_unique<Memory::Buffers::Local::RAMOnly<Bignum::Digit>>(largestSize);
		m_rBuffer = std::make_unique<Memory::Buffers::Local::RAMOnly<Bignum::Digit>>(largestSize);
		m_tmpBuffer = std::make_unique<Memory::Buffers::Local::RAMOnly<Bignum::Digit>>(
			BigFloat::getBufferSize(prec));

		m_tmpBigInt = std::make_unique<Bignum::BigInt>(m_tmpBuffer->accessData(0), largestSize);
		m_tmpBigFloat = std::make_unique<Bignum::BigFloat>(m_tmpBuffer->accessData(0), prec);

		// Now do the computation.
		// Another 3 FSVs, so we are at 5.5 FSVs total.
		rv.P = std::make_unique<Bignum::BigFloat>(prec);
		rv.Q = std::make_unique<Bignum::BigFloat>(prec);
		rv.R = std::make_unique<Bignum::BigFloat>(prec);

		giantSum(*rv.P, *rv.Q, *rv.R, a, b, numSteps);

		return rv;
	}

	// Private member.
	BSP::SmallOutput BSP::smallCompute(Memory::SafePtr<Bignum::Digit> pBufPtr,
		Memory::SafePtr<Bignum::Digit> qBufPtr, Memory::SafePtr<Bignum::Digit> rBufPtr,
		unsigned int a, unsigned int b)
	{
		// Prepare the output buffers.
		BSP::SmallOutput out;

		std::size_t pSizeEst(estimatePPrec(a, b));
		std::size_t qSizeEst(estimateQPrec(a, b));
		std::size_t rSizeEst(estimateRPrec(a, b));

		out.P = std::make_unique<Bignum::BigInt>(pBufPtr, pSizeEst);
		out.Q = std::make_unique<Bignum::BigInt>(qBufPtr, qSizeEst);
		out.R = std::make_unique<Bignum::BigInt>(rBufPtr, rSizeEst);

		if (b - a == 1) {
			// The recursion base case.
			p(*out.P, b);
			q(*out.Q, b);
			r(*out.R, b);

			// NB: add more cases
		} else {
			std::size_t m = (a + b) / 2;

			// note: these overlap with out, so we must be careful!
			BSP::SmallOutput Lout(BSP::smallCompute(pBufPtr, qBufPtr, rBufPtr, a, m));

			//std::cout << "P est: " << estimatePPrec(a, m) << " act: " << Lout.P->getDgsUsed() << std::endl;
			//std::cout << "Q est: " << estimateQPrec(a, m) << " act: " << Lout.Q->getDgsUsed() << std::endl;
			//std::cout << "R est: " << estimateRPrec(a, m) << " act: " << Lout.R->getDgsUsed() << std::endl;

			// Don't worry if we overwrite some parts of the bignums that aren't used, just as
			// long as everything stays within the size of out.P, out.Q, and out.R, which should
			// be the case so long as the estimator always overestimates.
			// This exploits that bn->getDgsUsed() is always equal to the number of significant
			// digits in the bignum.
			pBufPtr += Lout.P->getDgsUsed();
			qBufPtr += Lout.Q->getDgsUsed();
			rBufPtr += Lout.R->getDgsUsed();

			BSP::SmallOutput Rout(BSP::smallCompute(pBufPtr, qBufPtr, rBufPtr, m, b));

			//std::cout << "PP est: " << estimatePPrec(m, b) << " act: " << Rout.P->getDgsUsed() << std::endl;
			//std::cout << "QQ est: " << estimateQPrec(m, b) << " act: " << Rout.Q->getDgsUsed() << std::endl;
			//std::cout << "RR est: " << estimateRPrec(m, b) << " act: " << Rout.R->getDgsUsed() << std::endl;

			m_tmpBigInt->mul(*Rout.P, *Lout.R, *m_multiplicationStrategy);
			out.P->mul(*Lout.P, *Rout.Q, *m_multiplicationStrategy);
			out.P->addIp(*m_tmpBigInt);
			out.Q->mul(*Lout.Q, *Rout.Q, *m_multiplicationStrategy);
			out.R->mul(*Lout.R, *Rout.R, *m_multiplicationStrategy);
		}

		//std::cout << "P(" << a << ", " << b << ") = " << out.P->print() << std::endl;
		//std::cout << "Q(" << a << ", " << b << ") = " << out.Q->print() << std::endl;
		//std::cout << "R(" << a << ", " << b << ") = " << out.R->print() << std::endl;

		return out;
	}

	void BSP::giantSum(Bignum::BigFloat &P, Bignum::BigFloat &Q, Bignum::BigFloat &R, unsigned int a,
		unsigned int b, int numSteps)
	{
		// Do the first step and copy it into P and Q.
		// Note: Non-uniform stepping would likely be more efficient. This is just a start.
		unsigned int aCur(a), bCur(a + (b - a) / numSteps);

		Memory::SafePtr<Bignum::Digit> pWorkPtr(m_pBuffer->accessData(0));
		Memory::SafePtr<Bignum::Digit> qWorkPtr(m_qBuffer->accessData(0));
		Memory::SafePtr<Bignum::Digit> rWorkPtr(m_rBuffer->accessData(0));

		std::cout << "Computing step " << numSteps << std::endl;
		BSP::SmallOutput out(BSP::smallCompute(pWorkPtr, qWorkPtr, rWorkPtr, aCur, bCur));
		P.assign(*out.P);
		Q.assign(*out.Q);
		R.assign(*out.R);

		// Do the remaining steps.
		for (int i(2); i <= numSteps; ++i) {
			std::cout << "Computing step " << (numSteps + 1 - i) << std::endl;
			if (i == numSteps) {
				bCur = b;
			}

			// This is the giant merge.
			aCur = a + ((b - a) * (i - 1)) / numSteps;
			bCur = a + ((b - a) * i) / numSteps;
			BSP::SmallOutput out(BSP::smallCompute(pWorkPtr, qWorkPtr, rWorkPtr, aCur, bCur));
			P.mul(P, *out.Q, *m_multiplicationStrategy);
			m_tmpBigFloat->mul(R, *out.P, *m_multiplicationStrategy);
			P.addIp(*m_tmpBigFloat);
			Q.mul(Q, *out.Q, *m_multiplicationStrategy);
			R.mul(R, *out.R, *m_multiplicationStrategy);
		}
	}
}
