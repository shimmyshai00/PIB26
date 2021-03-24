/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      chudnovsky.cpp
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

#include "chudnovsky.hpp"

#include "../../util/timer.hpp"
#include "../../util/LabelTicker.hpp"

#include <iostream>
#include <cmath>

namespace SDF::Pi::BSP
{
	Chudnovsky::Chudnovsky(Bignum::IMultiplicationStrategy *multiplicationStrategy)
		: BSP(multiplicationStrategy), m_A(Bignum::DIGS_PER_SMALL), m_B(Bignum::DIGS_PER_SMALL), m_C(
			2 * Bignum::DIGS_PER_SMALL), m_smallTmp(2 * Bignum::DIGS_PER_SMALL)
	{
		m_A.assign(13591409);
		m_B.assign(545140134);
		m_C.assign(640320);
		m_C.mulIp(640320);
		m_C.mulIp(26680);
	}

	std::shared_ptr<Bignum::BigFloat> Chudnovsky::computePi(std::size_t numDigits)
	{
		struct timespec startTime, endTime;

		std::size_t prec(numDigits / Bignum::DIGS_PER_DIG);
		std::size_t numTerms(numDigits / 14 + 1);

		std::cout << "Computing series with " << numTerms << " terms..." << std::endl;
		clock_gettime(CLOCK_REALTIME, &startTime);
		BSPOutput bspResult(compute(0, numTerms, prec));
		clock_gettime(CLOCK_REALTIME, &endTime);
		std::cout << "Series computation complete. Time: " << Util::timeDiffMillis(endTime, startTime)
			<< " ms." << std::endl;
		std::cout << std::endl;

		// Use the BigFloats in the BSPOutput for scratch space.
		// The final result is:
		//     Pi = (4270934400 Q)/(P + 13591409 Q) [1/sqrt(10005)].
		Util::LabelTicker divTicker("Division");
		clock_gettime(CLOCK_REALTIME, &startTime);
		bspResult.Q->mulIp(13591409);
		bspResult.R->add(*bspResult.P, *bspResult.Q);
		bspResult.P->recip(*bspResult.R, *m_multiplicationStrategy, &divTicker);
		bspResult.Q->divIp(13591409);
		bspResult.Q->mulIp(4270934400U);

		bspResult.R->mul(*bspResult.Q, *bspResult.P, *m_multiplicationStrategy);
		clock_gettime(CLOCK_REALTIME, &endTime);
		std::cout << "Division complete. Time: " << Util::timeDiffMillis(endTime, startTime) << " ms."
			<< std::endl;
		std::cout << std::endl;

		Util::LabelTicker invSqrtTicker("InvSqrt");
		clock_gettime(CLOCK_REALTIME, &startTime);
		bspResult.Q->invsqrt(10005, *m_multiplicationStrategy, &invSqrtTicker);
		clock_gettime(CLOCK_REALTIME, &endTime);
		std::cout << "InvSqrt complete. Time: " << Util::timeDiffMillis(endTime, startTime) << " ms."
			<< std::endl;
		std::cout << std::endl;

		std::cout << "Performing final huge multiplication step..." << std::endl;
		clock_gettime(CLOCK_REALTIME, &startTime);
		bspResult.P->mul(*bspResult.Q, *bspResult.R, *m_multiplicationStrategy);
		clock_gettime(CLOCK_REALTIME, &endTime);
		std::cout << "Done! Time: " << Util::timeDiffMillis(endTime, startTime) << " ms."
			<< std::endl;
		std::cout << std::endl;

		return bspResult.P;
	}

	// Protected members.
	void Chudnovsky::p(Bignum::BigInt &res, unsigned int b)
	{
		m_smallTmp.mul(m_B, b);
		res.add(m_A, m_smallTmp);
		res.mulIp(2 * b - 1); // n.b. can overflow
		res.mulIp(6 * b - 5);
		res.mulIp(6 * b - 1);
		if (b % 2 == 1) {
			res.neg();
		}
	}

	void Chudnovsky::q(Bignum::BigInt &res, unsigned int b)
	{
		res.mul(m_C, b);
		res.mulIp(b);
		res.mulIp(b);
	}

	void Chudnovsky::r(Bignum::BigInt &res, unsigned int b)
	{
		res.assign(2 * b - 1);
		res.mulIp(6 * b - 5);
		res.mulIp(6 * b - 1);
	}

	std::size_t Chudnovsky::estimatePPrec(unsigned int a, unsigned int b) {
		// P is just a little smaller than Q.
		return estimateQPrec(a, b);
	}

	std::size_t Chudnovsky::estimateQPrec(unsigned int a, unsigned int b) {
		// This estimate is based on Stirling's approximation and is very close to exact.
		static const double logConst = log(10939058860032000.0);
		double aloga = (a == 0) ? 0 : a*log(1.0*a);

		return ceil(1 + ((b - a)*logConst + 3*b*log(1.0*b) - 3*aloga)/log(Bignum::BASE));
	}

	std::size_t Chudnovsky::estimateRPrec(unsigned int a, unsigned int b) {
		// this could use refinement
		return Chudnovsky::estimatePPrec(a, b);
	}
}
