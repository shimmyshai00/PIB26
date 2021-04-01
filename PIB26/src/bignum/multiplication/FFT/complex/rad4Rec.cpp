/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      rad4Rec.cpp
 * Timestamp: Mar 31, 2021
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

#include "rad4Rec.hpp"

#include "../../../../exceptions/exceptions.hpp"

#include "../FFTTweak.hpp"

#include <cmath>

namespace SDF::Bignum::Multiplication::Fft::Complex
{
	rad4Rec::rad4Rec(Memory::SafePtr<Cplex> omegaTable, std::size_t omegaSize)
		: m_omegaTable(omegaTable), m_omegaSize(omegaSize), m_iterativeFftThreshold(
			g_cacheSize / (2 * sizeof(Cplex))), // try to fit whole iterative
															// FFT - table and data - into
															// cache
		m_iterativeFft(omegaTable, omegaSize)
	{
	}

	std::size_t rad4Rec::getMaxFftSize() const
	{
		// The maximum FFT size is the largest power of 2 size dividing the size of the omega table.
		std::size_t maxSize(1);
		std::size_t omegaSize(m_omegaSize);
		while (!(omegaSize & 1)) {
			omegaSize >>= 1;
			maxSize <<= 1;
		}

		return maxSize;
	}

	std::size_t rad4Rec::getNearestSafeLengthTo(std::size_t length) const
	{
		// Get the smallest power of two just larger than this length. Since we switch to radix-2
		// iterative at the end, no need to check for divisibility by 4.
		std::size_t pow2Length(1);
		while (pow2Length < length) {
			pow2Length <<= 1;
		}

		if (m_omegaSize % pow2Length != 0) {
			throw Exceptions::Exception("ERROR: Required FFT size is too large!");
		} else {
			return pow2Length;
		}
	}

	void rad4Rec::doFwdTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		if (len < m_iterativeFftThreshold) {
			m_iterativeFft.doFwdTransform(data, len);
		} else {
			std::size_t quarter(len >> 2);
			std::size_t full(len);

			for (std::size_t n = 0; n < quarter; ++n) {
				Cplex w = m_omegaTable[n * m_omegaSize / full];
				Cplex w2 = m_omegaTable[2 * n * m_omegaSize / full];
				Cplex w3 = m_omegaTable[3 * n * m_omegaSize / full];

				Cplex d0 = data[n];
				Cplex d1 = data[n + quarter];
				Cplex d2 = data[n + 2 * quarter];
				Cplex d3 = data[n + 3 * quarter];

				// NB: we almost surely can do this in fewer operations
				//Cplex t0 = { d0.r + d1.r + d2.r + d3.r, d0.i + d1.i + d2.i + d3.i };
				//Cplex t1 = { d0.r + d1.i - d2.r - d3.i, d0.i - d1.r - d2.i + d3.r };
				//Cplex t2 = { d0.r - d1.r + d2.r - d3.r, d0.i - d1.i + d2.i - d3.i };
				//Cplex t3 = { d0.r - d1.i - d2.r + d3.i, d0.i + d1.r - d2.i - d3.r };

				// Optimized method: one should note that the "butterfly" is actually a small DFT
				//                   within the larger FFT, and thus we can use the FFT decomposition on
				//                   it as well.
				Cplex e0 = { d0.r + d2.r, d0.i + d2.i }; // two length-2 DFTs
				Cplex e1 = { d0.r - d2.r, d0.i - d2.i };
				Cplex o0 = { d1.r + d3.r, d1.i + d3.i };
				Cplex o1 = { d1.r - d3.r, d1.i - d3.i };

				Cplex t0 = { e0.r + o0.r, e0.i + o0.i };
				Cplex t1 = { e1.r + o1.i, e1.i - o1.r }; // e1 + (-i)*o1
				Cplex t2 = { e0.r - o0.r, e0.i - o0.i };
				Cplex t3 = { e1.r - o1.i, e1.i + o1.r }; // e1 - (-i)*o1

				// Trig power multiplication
				data[n].r = t0.r;
				data[n].i = t0.i;

				data[n + quarter].r = t1.r * w.r - t1.i * w.i;
				data[n + quarter].i = t1.r * w.i + t1.i * w.r;

				data[n + 2 * quarter].r = t2.r * w2.r - t2.i * w2.i;
				data[n + 2 * quarter].i = t2.r * w2.i + t2.i * w2.r;

				data[n + 3 * quarter].r = t3.r * w3.r - t3.i * w3.i;
				data[n + 3 * quarter].i = t3.r * w3.i + t3.i * w3.r;
			}

			doFwdTransform(data, quarter);
			doFwdTransform(data + quarter, quarter);
			doFwdTransform(data + 2*quarter, quarter);
			doFwdTransform(data + 3*quarter, quarter);
		}
	}

	void rad4Rec::doRevTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		if (len < m_iterativeFftThreshold) {
			m_iterativeFft.doRevTransform(data, len);
		} else {
			std::size_t quarter(len >> 2);
			std::size_t full(len);

			doRevTransform(data, quarter);
			doRevTransform(data + quarter, quarter);
			doRevTransform(data + 2*quarter, quarter);
			doRevTransform(data + 3*quarter, quarter);

			for (std::size_t k = 0; k < quarter; ++k) {
				Cplex w = m_omegaTable[k * m_omegaSize / full];
				Cplex w2 = m_omegaTable[2 * k * m_omegaSize / full];
				Cplex w3 = m_omegaTable[3 * k * m_omegaSize / full];

				w.i = -w.i;
				w2.i = -w2.i;
				w3.i = -w3.i;

				Cplex t0 = data[k];
				Cplex t1 = { data[k + quarter].r * w.r - data[k + quarter].i * w.i, data[k
					+ quarter].r * w.i + data[k + quarter].i * w.r };
				Cplex t2 = { data[k + 2 * quarter].r * w2.r - data[k + 2 * quarter].i * w2.i,
					data[k + 2 * quarter].r * w2.i + data[k + 2 * quarter].i * w2.r };
				Cplex t3 = { data[k + 3 * quarter].r * w3.r - data[k + 3 * quarter].i * w3.i,
					data[k + 3 * quarter].r * w3.i + data[k + 3 * quarter].i * w3.r };

				//Cplex d0 = { t0.r + t1.r + t2.r + t3.r, t0.i + t1.i + t2.i + t3.i };
				//Cplex d1 = { t0.r - t1.i - t2.r + t3.i, t0.i + t1.r - t2.i - t3.r };
				//Cplex d2 = { t0.r - t1.r + t2.r - t3.r, t0.i - t1.i + t2.i - t3.i };
				//Cplex d3 = { t0.r + t1.i - t2.r - t3.i, t0.i - t1.r - t2.i + t3.r };

				Cplex e0 = { t0.r + t2.r, t0.i + t2.i };
				Cplex e1 = { t0.r - t2.r, t0.i - t2.i };
				Cplex o0 = { t1.r + t3.r, t1.i + t3.i };
				Cplex o1 = { t1.r - t3.r, t1.i - t3.i };

				Cplex d0 = { e0.r + o0.r, e0.i + o0.i };
				Cplex d1 = { e1.r - o1.i, e1.i + o1.r }; // e1 + (+i)*o1
				Cplex d2 = { e0.r - o0.r, e0.i - o0.i };
				Cplex d3 = { e1.r + o1.i, e1.i - o1.r }; // e1 - (+i)*o1

				data[k] = d0;
				data[k + quarter] = d1;
				data[k + 2 * quarter] = d2;
				data[k + 3 * quarter] = d3;
			}
		}
	}
}

