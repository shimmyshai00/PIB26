/*
 * PIB26 version 0.0.2
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

#include "FFT/complex/genOmegaTable.hpp"
#include "FFT/complex/rad3Rec.hpp"

#include "../primitives/add.hpp"
#include "../primitives/addsm.hpp"
#include "../primitives/muladd.hpp"

#include <cmath>
#include <cstdio>

#include <iostream>
//#include <iomanip>
//#include <fstream>

namespace SDF::Bignum::Multiplication
{
	FFT::FFT(std::size_t maxProdSize)
		: m_maxProdSize(maxProdSize), m_lastProdLength(0)
	{
		std::size_t smallsPerElement(calcSmallsPerElement(m_maxProdSize));
		std::size_t expandedMaxProdSize((m_maxProdSize * DIGS_PER_DIG) / smallsPerElement);
		if ((expandedMaxProdSize * DIGS_PER_DIG) % smallsPerElement) {
			++expandedMaxProdSize; // round up
		}

		for (std::size_t i(0); i <= DIGS_PER_DIG; ++i) {
			m_smallBases[i] = pow(BASE_MINOR, i);
		}

		std::cout << "Preparing Fast Fourier Transform root tables ..." << std::flush;

		// The omega table size should always contain 1 factor of 3 for these FFTs.
		std::size_t omegaTableSize(3);
		while(omegaTableSize < expandedMaxProdSize) {
			omegaTableSize <<= 1;
		}

		// Create the omega table.
		m_omegaBuffer = Fft::Complex::genOmegaTable(omegaTableSize);

		std::cout << " done!" << std::endl;

		m_fft = std::make_unique<Fft::Complex::rad3Rec>(m_omegaBuffer->accessData(0), omegaTableSize);

		// Create the FFT buffers.
		m_fftBufferSize = m_fft->getNearestSafeLengthTo(expandedMaxProdSize);

		m_num1FFTBuffer = std::make_unique<Memory::Buffers::Local::RAMOnly<Fft::Complex::Cplex>>(
			m_fftBufferSize);
		m_num2FFTBuffer = std::make_unique<Memory::Buffers::Local::RAMOnly<Fft::Complex::Cplex>>(
			m_fftBufferSize);

		m_productDigits = m_num2FFTBuffer->pun<Digit>();
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
		Memory::SafePtr<Digit> num, std::size_t numLen, std::size_t smallsPerFftElement)
	{
		// The parameters fftBase and smallsPerFftElement allow us to stretch the FFT multiplication
		// to larger lengths than would otherwise be permitted by rounding error by packing fewer
		// small digits (i.e. base-BASE_MINOR) per element than the input, fed in base-BASE, has
		// directly. One should pass fftBase == BASE_MINOR^smallsPerFftElement; both are calculated by
		// getFFTBase.
		if (smallsPerFftElement == DIGS_PER_DIG) {
			// TBA: use right-angle convolution to save half the memory and transform length.
			for (std::size_t i(0); i < numLen; ++i) {
				fftBuffer[i] = { num[i], 0.0 };
			}

			for (std::size_t i(numLen); i < bufferLen; ++i) {
				fftBuffer[i] = { 0.0, 0.0 };
			}
		} else {
			// Pack fewer digits per element. We do this by in effect creating a "stream buffer" for
			// small digits, withdrawing smallsPerFftElement worth and loading it up again with new
			// digits when it gets low.
			TwoDigit smallDigitBuffer(0);
			std::size_t smallsInBuffer(0);
			std::size_t outBufIdx(0);

			for (std::size_t i(0); (i < numLen) && (outBufIdx < bufferLen); ++outBufIdx) {
				if (smallsInBuffer < smallsPerFftElement) {
					// "buffer" some new digits. Note we must add these to the _left_ of the ones
					// already in the buffer.
					smallDigitBuffer += m_smallBases[smallsInBuffer] * num[i];
					smallsInBuffer += DIGS_PER_DIG;
					++i;
				}

				// drain buffer
				fftBuffer[outBufIdx] = { (smallDigitBuffer % m_smallBases[smallsPerFftElement]), 0.0 };
				smallDigitBuffer /= m_smallBases[smallsPerFftElement];
				smallsInBuffer -= smallsPerFftElement;
			}

			// Finish off any remainder.
			for (; outBufIdx < bufferLen; ++outBufIdx) {
				fftBuffer[outBufIdx] = { smallDigitBuffer, 0.0 };
				smallDigitBuffer = 0;
			}
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
		Memory::SafePtr<Fft::Complex::Cplex> fftBuffer, std::size_t fftSize,
		std::size_t smallsPerFftElement)
	{
		if (smallsPerFftElement == DIGS_PER_DIG) {
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
		} else {
			// Repack the elements while extracting.
			double carry(0);
			TwoDigit smallDigitBuffer(0);
			std::size_t smallsInBuffer(0);
			std::size_t outBufIdx(0);
			for (std::size_t i(0); (i < fftSize) && (outBufIdx < digitsToGet); ++i) {
				// divide by the FFT size; the FFT leaves the product terms multiplied by it
				double tmp = floor((fftBuffer[i].r / fftSize) + 0.5);

				// release carries
				tmp += carry;
				carry = floor(tmp / m_smallBases[smallsPerFftElement]);
				tmp -= carry * m_smallBases[smallsPerFftElement];

				// buffer it
				smallDigitBuffer += m_smallBases[smallsInBuffer] * tmp;
				smallsInBuffer += smallsPerFftElement;

				if (smallsInBuffer >= DIGS_PER_DIG) {
					digitBuffer[outBufIdx] = smallDigitBuffer % BASE;
					smallDigitBuffer /= BASE;
					smallsInBuffer -= DIGS_PER_DIG;
					++outBufIdx;
				}
			}

			for (; outBufIdx < digitsToGet; ++outBufIdx) {
				digitBuffer[outBufIdx] = smallDigitBuffer;
				smallDigitBuffer = 0;
			}
		}
	}

// Private helper members.
	std::size_t FFT::calcSmallsPerElement(std::size_t prodSize)
	{
		// This is based on the observation that the size of the largest element in the multiplication
		// pyramid is given by:
		//
		//    Pyramid Max = (factorSize) * (BASE-1)^2
		//
		// so that the maximum permitted multiplication at a given base BASE is
		//
		//    Max Factor Size = 2^(53 - 2 lg(BASE-1))
		//
		// and we use hand tweaked factors so we can adjust for the possibility of any extra
		// rounding error in the FFT math itself.

		// Note: if BASE is changed, one will need to recalculate these factors. These assume
		//       BASE == 456976. If we want to change BASE frequently, we might want to automate this.
		std::size_t factorSize((prodSize / 2) + (prodSize % 2));
		if (factorSize <= 4096) {
			return 4;
		} else if (factorSize <= 2097152) {
			return 3;
		} else {
			return 2;
		}
	}

// Private member.
	void FFT::mulCore(Memory::SafePtr<Digit> a, std::size_t aLen, Memory::SafePtr<Digit> b,
		std::size_t bLen)
	{
		// Load the numbers into the buffers.
		std::size_t prodSize(aLen + bLen);
		std::size_t smallsPerElement(calcSmallsPerElement(prodSize));
		std::size_t expandedProdSize = (prodSize * DIGS_PER_DIG) / smallsPerElement;
		if ((prodSize * DIGS_PER_DIG) % smallsPerElement) {
			++expandedProdSize; // round up
		}
		std::size_t safeSize(m_fft->getNearestSafeLengthTo(expandedProdSize));

		Memory::SafePtr<Fft::Complex::Cplex> num1BufPtr(m_num1FFTBuffer->accessData(0));
		Memory::SafePtr<Fft::Complex::Cplex> num2BufPtr(m_num2FFTBuffer->accessData(0));

		loadBuffer(num1BufPtr, safeSize, a, aLen, smallsPerElement);
		loadBuffer(num2BufPtr, safeSize, b, bLen, smallsPerElement);

		// Do the FFT multiplication itself.
		m_fft->doFwdTransform(num1BufPtr, safeSize);
		m_fft->doFwdTransform(num2BufPtr, safeSize);

		convolute(num1BufPtr, num2BufPtr, safeSize);

		m_fft->doRevTransform(num1BufPtr, safeSize);

		// Unspool the result and release the carries.
		Memory::SafePtr<Digit> resultPtr(m_productDigits.accessData(0));
		extractProduct(resultPtr, prodSize, num1BufPtr, safeSize, smallsPerElement);
	}

	void FFT::sqrCore(Memory::SafePtr<Digit> a, std::size_t aLen)
	{
		// Can save a transform when squaring
		std::size_t prodSize(aLen << 1);
		std::size_t smallsPerElement(calcSmallsPerElement(prodSize));
		std::size_t expandedProdSize = (prodSize * DIGS_PER_DIG) / smallsPerElement;
		if ((prodSize * DIGS_PER_DIG) % smallsPerElement) {
			++expandedProdSize; // round up
		}
		std::size_t safeSize(m_fft->getNearestSafeLengthTo(expandedProdSize));

		Memory::SafePtr<Fft::Complex::Cplex> num1BufPtr(m_num1FFTBuffer->accessData(0));

		loadBuffer(num1BufPtr, safeSize, a, aLen, smallsPerElement);

		m_fft->doFwdTransform(num1BufPtr, safeSize);
		convolute(num1BufPtr, num1BufPtr, safeSize);
		m_fft->doRevTransform(num1BufPtr, safeSize);

		Memory::SafePtr<Digit> resultPtr(m_productDigits.accessData(0));
		extractProduct(resultPtr, prodSize, num1BufPtr, safeSize, smallsPerElement);
	}
}
