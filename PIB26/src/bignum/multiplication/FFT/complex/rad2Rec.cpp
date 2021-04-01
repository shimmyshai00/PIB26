/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      rad2Rec.cpp
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

#include "rad2Rec.hpp"

#include "../../../../exceptions/exceptions.hpp"

#include "../FFTTweak.hpp"

#include <cmath>

namespace SDF::Bignum::Multiplication::Fft::Complex
{
	rad2Rec::rad2Rec(Memory::SafePtr<Cplex> omegaTable, std::size_t omegaSize)
		: m_omegaTable(omegaTable), m_omegaSize(omegaSize),
		  m_rad4Fft(omegaTable, omegaSize)
	{
	}

	std::size_t rad2Rec::getMaxFftSize() const
	{
		// The maximum FFT size is the largest power of 2 size dividing the size of the omega table.
		std::size_t maxSize(1);
		std::size_t omegaSize(m_omegaSize);
		while(!(omegaSize & 1)) {
			omegaSize >>= 1;
			maxSize <<= 1;
		}

		return maxSize;
	}

	std::size_t rad2Rec::getNearestSafeLengthTo(std::size_t length) const
	{
		// Get the smallest power of two just larger than this length.
		std::size_t pow2Length(1);
		while (pow2Length < length) {
			pow2Length <<= 1;
		}

		if(m_omegaSize % pow2Length != 0) {
			throw Exceptions::Exception("ERROR: Required FFT size is too large!");
		} else {
			return pow2Length;
		}
	}

	void rad2Rec::doFwdTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		// The forward transform is done as a decimation-in-frequency (DIF) version.
		// The DIF version in a sense is just "built backwards" from the DIT version.
		if (len & 0x5555555555555555UL) { // quick check for even powers of 2 NB: needs recalibrating
			                               // for 32-bit machines
			// Right now, we only use the radix-2 to break an odd power of 2 so we can then use the
			// radix-4.
			m_rad4Fft.doFwdTransform(data, len);
		} else {
			std::size_t half(len >> 1);
			std::size_t full(len);

			for (std::size_t n = 0; n < half; ++n) {
				// NEW: use precomputed trig tables.
				Cplex w = m_omegaTable[n * m_omegaSize / full];

				Cplex tmp1 = { data[n].r + data[n + half].r, data[n].i + data[n + half].i };
				Cplex tmp2 = { data[n].r - data[n + half].r, data[n].i - data[n + half].i };

				data[n].r = tmp1.r;
				data[n].i = tmp1.i;

				data[n + half].r = tmp2.r * w.r - tmp2.i * w.i;
				data[n + half].i = tmp2.r * w.i + tmp2.i * w.r;
			}

			doFwdTransform(data, half);
			doFwdTransform(data + half, half);
		}
	}

	void rad2Rec::doRevTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		// Effectively assumes the data has been already sorted into even and odd parts, so that
		// data(R/I) is the even part and data(R/I) + 2^(pow / 2) is the odd part. This way we can
		// avoid the wasteful scrambling part by combining with the DIF code above. This makes the
		// FFT useless as a frequency analyzer - if one wants to recycle this code in another program
		// where it has that use, one will need to adapt the implementation or fashion a suitable
		// frontend that will recursively sort the data.
		if (len & 0x5555555555555555UL) {
			m_rad4Fft.doRevTransform(data, len);
		} else {
			// Do the recursion:
			//    X_k =                sum_{m=0...N/2-1} x_{2m} e^(-2piimk/(N/2))
			//          + e^(-2piik/N) sum_{m=0...N/2-1} x_{2m+1} e^(-2piimk/(N/2))
			// which gives
			//
			//    X_k =         x_E_k + e^(-2piik/N) x_O_k
			//    X_{k + N/2} = x_E_k - e^(-2piik/N) x_O_k.
			std::size_t half(len >> 1);
			std::size_t full(len);

			doRevTransform(data, half);
			doRevTransform(data + half, half);

			for (std::size_t k = 0; k < half; ++k) {
				Cplex w = m_omegaTable[k * m_omegaSize / full];
				w.i = -w.i;

				Cplex tmp1 { data[k].r, data[k].i };
				Cplex tmp2 { data[k + half].r * w.r - data[k + half].i * w.i, data[k + half].r * w.i
					+ data[k + half].i * w.r };

				data[k].r = tmp1.r + tmp2.r;
				data[k].i = tmp1.i + tmp2.i;

				data[k + half].r = tmp1.r - tmp2.r;
				data[k + half].i = tmp1.i - tmp2.i;
			}
		}
	}
}
