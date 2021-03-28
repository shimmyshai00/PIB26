/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      FFT.hpp
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

#ifndef SRC_BIGNUM_MULTIPLICATION_FFT_HPP_
#define SRC_BIGNUM_MULTIPLICATION_FFT_HPP_

#include "../IMultiplicationStrategy.hpp"

#include "../../memory/buffers/local/RAMOnly.hpp"

#include "../../memory/buffers/local/Punned.hpp"

#include "FFT/complex/Cplex.hpp"

#include "FFT/complex/rad2Rec.hpp"

#include <memory>

namespace SDF::Bignum::Multiplication
{
	// Class:      FFT
	// Purpose:    Performs a multiplication using the Fast Fourier Transform (FFT). This class of
	//             methods is the asymptotically fastest kind of multiplication method, with
	//             O(n log n) theoretical complexity, but in practice slightly worse due to precision
	//             considerations (namely, because the FFT cannot handle internal carrying, carries
	//             in the multiplication are not released until the end, which means they build up
	//             and the largest elements can overflow the size of the primitive data type
	//             representing that element. There are a number of algorithms to defeat this, but
	//             as it has to adapt, it causes the complexity to get worse). This particular method
	//             performs FFTs on the complex field. TBA: finite field (NTT), rings Z_(2^N+1)
	//             (aka Schonhage-Strassen method) for extremely huge computations.
	// Parameters: None.
	class FFT : public IMultiplicationStrategy
	{
		public:
			// Function:  FFT
			// Purpose:   Construct a new strategy object with a given maximum buffer size.
			// Arguments: maxProdSize - The maximum multiplication size to allow.
			FFT(std::size_t maxProdSize);

			void mulDigits(Memory::SafePtr<Digit> a, std::size_t aLen, Memory::SafePtr<Digit> b,
				std::size_t bLen);
			void squareDigits(Memory::SafePtr<Digit> a, std::size_t aLen);

			std::size_t getProductLength() const;

			void getProductDigits(Memory::SafePtr<Digit> dst, std::size_t origin, std::size_t length);
		private:
			static const std::size_t THRESHOLD_SMOOTHING = 4;

			// The FFT object itself.
			std::unique_ptr<Fft::IFft<Fft::Complex::Cplex>> m_fft;

			std::size_t m_maxProdSize;
			std::size_t m_fftBufferSize;
			std::size_t m_lastProdLength;

			// Use punning to save storage space
			Memory::Buffers::Local::Punned<Digit> m_productDigits;

			Memory::Buffers::Local::RAMOnly<Fft::Complex::Cplex> m_num1FFTBuffer;
			Memory::Buffers::Local::RAMOnly<Fft::Complex::Cplex> m_num2FFTBuffer;

			std::size_t calcBufferSize(std::size_t maxProdSize);
			std::size_t calcAbsoluteMaxProdSize();
			std::pair<Digit, std::size_t> calcFFTBase(std::size_t prodSize);

			void loadBuffer(Memory::SafePtr<Fft::Complex::Cplex> fftBuffer, std::size_t bufferLen,
				Memory::SafePtr<Digit> num, std::size_t numLen);
			void convolute(Memory::SafePtr<Fft::Complex::Cplex> fftBuffer1,
				Memory::SafePtr<Fft::Complex::Cplex> fftBuffer2, std::size_t bufferLen);
			void extractProduct(Memory::SafePtr<Digit> digitBuffer, std::size_t digitsToGet,
				Memory::SafePtr<Fft::Complex::Cplex> fftBuffer, std::size_t fftSize);

			void mulCore(Memory::SafePtr<Digit> a, std::size_t aLen, Memory::SafePtr<Digit> b,
				std::size_t bLen);
			void sqrCore(Memory::SafePtr<Digit> a, std::size_t aLen);
	};
}

#endif /* SRC_BIGNUM_MULTIPLICATION_FFT_HPP_ */

