/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 *
 * File:      FFT.cpp
 * Timestamp: Mar 23, 2021
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

#include "FFT.hpp"

#include "../../exceptions/exceptions.hpp"

#include "FFT/complex/rad3Rec.hpp"

#include "../primitives/add.hpp"
#include "../primitives/addsm.hpp"
#include "../primitives/muladd.hpp"

#include <cmath>
#include <cstdio>

//#include <iostream>
//#include <iomanip>
//#include <fstream>

namespace SDF::Bignum::Multiplication
{
	FFT::FFT(std::size_t maxProdSize)
		: m_fft(new Fft::Complex::rad3Rec), m_maxProdSize(maxProdSize), m_fftBufferSize(
			calcBufferSize(maxProdSize)), m_lastProdLength(0), m_num1FFTBuffer(
			m_fftBufferSize), m_num2FFTBuffer(m_fftBufferSize)
	{
		if (m_fftBufferSize > 2 * m_fft->getMaxNumLengthAtBase(calcFFTBase(maxProdSize).first)) {
			// Bad!
			throw SDF::Exceptions::Exception("Required FFT multiply size too big!");
		}

		m_productDigits = m_num2FFTBuffer.pun<Digit>();
	}

	void FFT::mulDigits(Memory::SafePtr<Digit> a, std::size_t aLen, Memory::SafePtr<Digit> b,
		std::size_t bLen)
	{
		if (aLen + bLen > m_maxProdSize) {
			throw SDF::Exceptions::Exception("Requested FFT multiply of numbers that were too big :(");
		}

		// Because the FFT does not permit arbitrary lengths without padding, performance
		// "thresholding" may be a problem, particularly when dealing with multiplies that, by some
		// other effect, are coincident with the size steps - most notably, in the Newton routines. To
		// deal with this, if we get a request to multiply two numbers very close to, but above, a
		// size threshold, we'd like to use the next smaller transform and then finish out the job
		// with a few classical multiply passes.
		std::size_t prodSize(aLen + bLen);
		std::size_t pow2Size(1);

		while ((pow2Size << 1) < prodSize) {
			pow2Size <<= 1;
		}

		if (prodSize - pow2Size < 2 * THRESHOLD_SMOOTHING) {
			Memory::SafePtr<Digit> prodPtr(m_productDigits.accessData(0));

			// "Smooth" the performance by multiplying THRESHOLD_SMOOTHING fewer digits using the FFT.
			std::size_t aTrunc(aLen - THRESHOLD_SMOOTHING);
			std::size_t bTrunc(bLen - THRESHOLD_SMOOTHING);
			mulCore(a, aTrunc, b, bTrunc);

			for (std::size_t i(aTrunc + bTrunc); i < prodSize; ++i) {
				prodPtr[i] = 0;
			}

			// Now fill in for it with a few classical mul passes.
			// We use the formula
			// (d_1 B^n + X)(d_2 B^m + Y) = d_1 d_2 B^(n + m) + d_1 Y B^n + d_2 X B^m + XY
			// where X and Y are the truncated factors, and d_1 and d_2 are the missing digits.
			//for (std::size_t i(prodSize); i > 0; --i) {
			//std::cout << std::setw(4) << std::setfill('0') << prodPtr[i-1];
			//}
			//std::cout << std::endl;

			for (std::size_t i(THRESHOLD_SMOOTHING); i > 0; --i) {
				aTrunc = aLen - i;
				bTrunc = bLen - i;

				TwoDigit bigCarry(0);
				bigCarry = Primitives::mulAdd(prodPtr + aTrunc, prodPtr + aTrunc, b, a[aTrunc], bTrunc);
				Primitives::addSmall(prodPtr + aTrunc + bTrunc, prodPtr + aTrunc + bTrunc, bigCarry,
					prodSize - (aTrunc + bTrunc));

				bigCarry = Primitives::mulAdd(prodPtr + bTrunc, prodPtr + bTrunc, a, b[bTrunc], aTrunc);
				Primitives::addSmall(prodPtr + bTrunc + aTrunc, prodPtr + bTrunc + aTrunc, bigCarry,
					prodSize - (bTrunc + aTrunc));

				bigCarry = static_cast<TwoDigit>(a[aTrunc]) * b[bTrunc];
				Primitives::addSmall(prodPtr + aTrunc + bTrunc, prodPtr + aTrunc + bTrunc, bigCarry,
					prodSize - (aTrunc + bTrunc));
			}
		} else {
			mulCore(a, aLen, b, bLen);
		}

		// Report the product size.
		m_lastProdLength = prodSize;
		if (*m_productDigits.accessData(m_lastProdLength - 1) == 0) {
			--m_lastProdLength;
		}
	}

	void FFT::squareDigits(Memory::SafePtr<Digit> a, std::size_t aLen)
	{
		// We can save an FFT in this case.
		if ((aLen << 1) > m_maxProdSize) {
			throw SDF::Exceptions::Exception("Requested FFT multiply of numbers that were too big :(");
		}

		std::size_t prodSize(aLen << 1);
		std::size_t pow2Size(1);

		while ((pow2Size << 1) < prodSize) {
			pow2Size <<= 1;
		}

		if (prodSize - pow2Size < 2 * THRESHOLD_SMOOTHING) {
			Memory::SafePtr<Digit> prodPtr(m_productDigits.accessData(0));

			// "Smooth" the performance by multiplying THRESHOLD_SMOOTHING fewer digits using the FFT.
			std::size_t aTrunc(aLen - THRESHOLD_SMOOTHING);
			sqrCore(a, aTrunc);

			for (std::size_t i(aTrunc << 1); i < prodSize; ++i) {
				prodPtr[i] = 0;
			}

			for (std::size_t i(THRESHOLD_SMOOTHING); i > 0; --i) {
				aTrunc = aLen - i;

				TwoDigit bigCarry(0);
				bigCarry = Primitives::mulAdd(prodPtr + aTrunc, prodPtr + aTrunc, a, 2 * a[aTrunc],
					aTrunc);
				Primitives::addSmall(prodPtr + aTrunc + aTrunc, prodPtr + aTrunc + aTrunc, bigCarry,
					prodSize - (aTrunc + aTrunc));

				bigCarry = static_cast<TwoDigit>(a[aTrunc]) * a[aTrunc];
				Primitives::addSmall(prodPtr + aTrunc + aTrunc, prodPtr + aTrunc + aTrunc, bigCarry,
					prodSize - (aTrunc + aTrunc));
			}
		} else {
			sqrCore(a, aLen);
		}

		// Report the product size.
		m_lastProdLength = prodSize;
		if (*m_productDigits.accessData(m_lastProdLength - 1) == 0) {
			--m_lastProdLength;
		}
	}

	std::size_t FFT::getProductLength() const
	{
		return m_lastProdLength;
	}

	void FFT::getProductDigits(Memory::SafePtr<Digit> dst, std::size_t origin, std::size_t length)
	{
		Memory::SafePtr<Digit> digitPtr(m_productDigits.accessData(origin));

		for (std::size_t i(0); i < length; ++i) {
			dst[i] = digitPtr[i];
		}
	}

	// Private members.
	void FFT::loadBuffer(Memory::SafePtr<Fft::Complex::Cplex> fftBuffer, std::size_t bufferLen,
		Memory::SafePtr<Digit> num, std::size_t numLen)
	{
		// Super naive - not even right angle convolution yet in this very first rough draft
		for (std::size_t i(0); i < numLen; ++i) {
			fftBuffer[i] = { num[i], 0.0 };
		}

		for (std::size_t i(numLen); i < bufferLen; ++i) {
			fftBuffer[i] = { 0.0, 0.0 };
		}
	}

	void FFT::convolute(Memory::SafePtr<Fft::Complex::Cplex> fftBuffer1,
		Memory::SafePtr<Fft::Complex::Cplex> fftBuffer2, std::size_t bufferLen)
	{
		for (std::size_t i(0); i < bufferLen; ++i) {
			double tmpR = fftBuffer1[i].r * fftBuffer2[i].r - fftBuffer1[i].i * fftBuffer2[i].i;
			double tmpI = fftBuffer1[i].r * fftBuffer2[i].i + fftBuffer1[i].i * fftBuffer2[i].r;

			fftBuffer1[i].r = tmpR;
			fftBuffer1[i].i = tmpI;
		}
	}

	void FFT::extractProduct(Memory::SafePtr<Digit> digitBuffer, std::size_t digitsToGet,
		Memory::SafePtr<Fft::Complex::Cplex> fftBuffer, std::size_t fftSize)
	{
		double carry(0);
		for (std::size_t i(0); i < digitsToGet; ++i) {
			// divide by the FFT size; the FFT leaves the product terms multiplied by it
			double tmp = floor((fftBuffer[i].r / fftSize) + 0.5);

			// release carries
			tmp += carry;
			carry = floor(tmp / BASE);
			tmp -= carry * BASE;

			digitBuffer[i] = tmp;
		}
	}

	// Private helper members.
	std::size_t FFT::calcBufferSize(std::size_t maxProdSize) {
		// Naive method: just pack one digit per FFT element. At a base BASE = 10000, this will
		// generally fail some time past 32M digits, so to go further with this program, we will need
		// tweaking here to pack fewer digits per element and suitable buffer sizing.
		return m_fft->getNearestSafeLengthTo(maxProdSize);
	}

	std::pair<Digit, std::size_t> FFT::calcFFTBase(std::size_t prodSize) {
		// Naive: just use the same base
		return std::make_pair(BASE, DIGS_PER_DIG);
	}

	// Private member.
	void FFT::mulCore(Memory::SafePtr<Digit> a, std::size_t aLen, Memory::SafePtr<Digit> b,
		std::size_t bLen)
	{
		// Load the numbers into the buffers.
		std::size_t prodSize(aLen + bLen);
		std::size_t safeSize(m_fft->getNearestSafeLengthTo(prodSize));

		Memory::SafePtr<Fft::Complex::Cplex> num1BufPtr(m_num1FFTBuffer.accessData(0));
		Memory::SafePtr<Fft::Complex::Cplex> num2BufPtr(m_num2FFTBuffer.accessData(0));

		loadBuffer(num1BufPtr, safeSize, a, aLen);
		loadBuffer(num2BufPtr, safeSize, b, bLen);

		// Do the FFT multiplication itself.
		m_fft->doFwdTransform(num1BufPtr, safeSize);
		m_fft->doFwdTransform(num2BufPtr, safeSize);

		convolute(num1BufPtr, num2BufPtr, safeSize);

		m_fft->doRevTransform(num1BufPtr, safeSize);

		// Unspool the result and release the carries.
		Memory::SafePtr<Digit> resultPtr(m_productDigits.accessData(0));
		extractProduct(resultPtr, prodSize, num1BufPtr, safeSize);
	}

	void FFT::sqrCore(Memory::SafePtr<Digit> a, std::size_t aLen)
	{
		// Can save a transform when squaring
		std::size_t prodSize(aLen << 1);
		std::size_t safeSize(m_fft->getNearestSafeLengthTo(prodSize));

		Memory::SafePtr<Fft::Complex::Cplex> num1BufPtr(m_num1FFTBuffer.accessData(0));

		loadBuffer(num1BufPtr, safeSize, a, aLen);

		m_fft->doFwdTransform(num1BufPtr, safeSize);
		convolute(num1BufPtr, num1BufPtr, safeSize);
		m_fft->doRevTransform(num1BufPtr, safeSize);

		Memory::SafePtr<Digit> resultPtr(m_productDigits.accessData(0));
		extractProduct(resultPtr, prodSize, num1BufPtr, safeSize);
	}
}
