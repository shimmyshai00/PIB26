/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      rad3Rec.cpp
 * Timestamp: Mar 25, 2021
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

#include "rad3Rec.hpp"

#include "../../../../exceptions/exceptions.hpp"

#include <cmath>
#include <cstdio>

namespace SDF::Bignum::Multiplication::Fft::Complex
{
	rad3Rec::rad3Rec(Memory::SafePtr<Cplex> omegaTable, std::size_t omegaSize)
		: m_omegaTable(omegaTable), m_omegaSize(omegaSize), m_rad2Fft(omegaTable, omegaSize)
	{
	}

	std::size_t rad3Rec::getMaxFftSize() const {
		// The maximum FFT size for this type of transform is the largest number of the form 2^n or
		// 3 * 2^n that divides the size of the omega table.
		std::size_t maxPow2(1);
		std::size_t omegaSize(m_omegaSize);
		while(!(omegaSize & 1)) {
			omegaSize >>= 1;
			maxPow2 <<= 1;
		}

		if(m_omegaSize % (3 * maxPow2) == 0) {
			return 3 * maxPow2;
		} else {
			return maxPow2;
		}
	}

	std::size_t rad3Rec::getNearestSafeLengthTo(std::size_t length) const
	{
		if(length > m_omegaSize) {
			throw Exceptions::Exception("ERROR: Required FFT size is too large!");
		} else {
			// Get smallest power of 2 no smaller than length.
			std::size_t pow2(1);
			while(pow2 < length) {
				pow2 <<= 1;
			}

			// Get smallest number of the form 3 * 2^n no smaller than length.
			std::size_t pow2Times3(3);
			while(pow2Times3 < length) {
				pow2Times3 <<= 1;
			}

			// Use the smaller as the desired safe length.
			if((pow2 < pow2Times3) && (m_omegaSize % pow2 == 0)) {
				return pow2;
			} else {
				return pow2Times3;
			}
		}
	}

	void rad3Rec::doFwdTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		if (len % 3 != 0) {
			// Switch to radix-2 transform.
			m_rad2Fft.doFwdTransform(data, len);
		} else {
			std::size_t third(len / 3);
			std::size_t full(len);

			for (std::size_t n = 0; n < third; ++n) {
				Cplex w = m_omegaTable[n * m_omegaSize / full];
				Cplex w2 = w; // square of w

				if(2 * n * m_omegaSize / full < m_omegaSize) {
					w2 = m_omegaTable[2 * n * m_omegaSize / full];
				} else {
					w2.r = w.r * w.r - w.i * w.i;
					w2.i = 2.0 * w.r * w.i;
				}

				static const double c_sq3o2 = 0.8660254037844386l; // sqrt(3)/2
				Cplex tmp1, tmp2, tmp3;

				// Tip: generate these with a CAS; not by hand!
				tmp1.r = data[n].r + data[n + third].r + data[n + 2 * third].r;
				tmp1.i = data[n].i + data[n + third].i + data[n + 2 * third].i;

				tmp2.r = data[n].r - 0.5 * (data[n + third].r + data[n + 2 * third].r)
					+ c_sq3o2 * (data[n + third].i - data[n + 2 * third].i);
				tmp2.i = data[n].i - 0.5 * (data[n + third].i + data[n + 2 * third].i)
					- c_sq3o2 * (data[n + third].r - data[n + 2 * third].r);

				tmp3.r = data[n].r - 0.5 * (data[n + third].r + data[n + 2 * third].r)
					- c_sq3o2 * (data[n + third].i - data[n + 2 * third].i);
				tmp3.i = data[n].i - 0.5 * (data[n + third].i + data[n + 2 * third].i)
					+ c_sq3o2 * (data[n + third].r - data[n + 2 * third].r);

				data[n].r = tmp1.r;
				data[n].i = tmp1.i;

				data[n + third].r = tmp2.r * w.r - tmp2.i * w.i;
				data[n + third].i = tmp2.r * w.i + tmp2.i * w.r;

				data[n + 2 * third].r = tmp3.r * w2.r - tmp3.i * w2.i;
				data[n + 2 * third].i = tmp3.r * w2.i + tmp3.i * w2.r;
			}

			doFwdTransform(data, third);
			doFwdTransform(data + third, third);
			doFwdTransform(data + 2 * third, third);
		}
	}

	void rad3Rec::doRevTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		if (len % 3 != 0) {
			// Switch to radix-2 transform.
			m_rad2Fft.doRevTransform(data, len);
		} else {
			std::size_t third(len / 3);
			std::size_t full(len);

			doRevTransform(data, third);
			doRevTransform(data + third, third);
			doRevTransform(data + 2 * third, third);

			for (std::size_t n = 0; n < third; ++n) {
				Cplex w = m_omegaTable[n * m_omegaSize / full];
				w.i = -w.i; // invert the root
				Cplex w2 = w; // square of w

				if(2 * n * m_omegaSize / full < m_omegaSize) {
					w2 = m_omegaTable[2 * n * m_omegaSize / full];
					w2.i = -w2.i;
				} else {
					w2.r = w.r * w.r - w.i * w.i;
					w2.i = 2.0 * w.r * w.i;
				}

				static const double c_sq3o2 = 0.8660254037844386l; // sqrt(3)/2
				Cplex tmp1, tmp2, tmp3;

				tmp1.r = data[n].r;
				tmp1.i = data[n].i;

				tmp2.r = data[n + third].r * w.r - data[n + third].i * w.i;
				tmp2.i = data[n + third].r * w.i + data[n + third].i * w.r;

				tmp3.r = data[n + 2 * third].r * w2.r - data[n + 2 * third].i * w2.i;
				tmp3.i = data[n + 2 * third].r * w2.i + data[n + 2 * third].i * w2.r;

				data[n].r = tmp1.r + tmp2.r + tmp3.r;
				data[n].i = tmp1.i + tmp2.i + tmp3.i;

				// n.b. some of the signs change here due to the conjugated trig
				data[n + third].r = tmp1.r - 0.5 * (tmp2.r + tmp3.r) - c_sq3o2 * (tmp2.i - tmp3.i);
				data[n + third].i = tmp1.i - 0.5 * (tmp2.i + tmp3.i) + c_sq3o2 * (tmp2.r - tmp3.r);

				data[n + 2 * third].r = tmp1.r - 0.5 * (tmp2.r + tmp3.r) + c_sq3o2 * (tmp2.i - tmp3.i);
				data[n + 2 * third].i = tmp1.i - 0.5 * (tmp2.i + tmp3.i) - c_sq3o2 * (tmp2.r - tmp3.r);
			}
		}
	}
}
