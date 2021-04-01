/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      rad4Itr.cpp
 * Timestamp: Apr 1, 2021
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

#include "rad4Itr.hpp"

#include "../../../../exceptions/exceptions.hpp"

#include <cmath>
#include <cstdio>

namespace SDF::Bignum::Multiplication::Fft::Complex
{
	rad4Itr::rad4Itr(Memory::SafePtr<Cplex> omegaTable, std::size_t omegaSize)
		: m_omegaTable(omegaTable), m_omegaSize(omegaSize)
	{
	}

	std::size_t rad4Itr::getMaxFftSize() const
	{
		// The maximum FFT size is the largest power of 4 size dividing the size of the omega table.
		std::size_t maxSize(1);
		std::size_t omegaSize(m_omegaSize);
		while (!(omegaSize & 2)) {
			omegaSize >>= 2;
			maxSize <<= 2;
		}

		return maxSize;
	}

	std::size_t rad4Itr::getNearestSafeLengthTo(std::size_t length) const
	{
		// Get the smallest power of 4 just larger than this length.
		std::size_t pow4Length(1);
		while (pow4Length < length) {
			pow4Length <<= 2;
		}

		if (m_omegaSize % pow4Length != 0) {
			throw Exceptions::Exception("ERROR: Required FFT size is too large!");
		} else {
			return pow4Length;
		}
	}

	void rad4Itr::doFwdTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		std::size_t step(len);
		std::size_t omegaStep(m_omegaSize / len); // for exploiting that even though the table stores
																// e^(-2piin/Nmax) for some Nmax, we have
																// e^(-2piin/(Nmax/d)) = e^(-2pii(dn)/Nmax).

		while (step >= 4) {
			// Perform the same pass as in the recursive case but over chunks of size step.
			for (Memory::SafePtr<Cplex> chunk(data); chunk < data + len; chunk += step) {
				std::size_t quarter(step >> 2);

				for (std::size_t n = 0; n < quarter; ++n) {
					Cplex w = m_omegaTable[n * omegaStep];
					Cplex w2 = m_omegaTable[2 * n * omegaStep];
					Cplex w3 = m_omegaTable[3 * n * omegaStep];

					Cplex d0 = chunk[n];
					Cplex d1 = chunk[n + quarter];
					Cplex d2 = chunk[n + 2 * quarter];
					Cplex d3 = chunk[n + 3 * quarter];

					Cplex e0 = { d0.r + d2.r, d0.i + d2.i };
					Cplex e1 = { d0.r - d2.r, d0.i - d2.i };
					Cplex o0 = { d1.r + d3.r, d1.i + d3.i };
					Cplex o1 = { d1.r - d3.r, d1.i - d3.i };

					Cplex t0 = { e0.r + o0.r, e0.i + o0.i };
					Cplex t1 = { e1.r + o1.i, e1.i - o1.r }; // e1 + (-i)*o1
					Cplex t2 = { e0.r - o0.r, e0.i - o0.i };
					Cplex t3 = { e1.r - o1.i, e1.i + o1.r }; // e1 - (-i)*o1

					// Trig power multiplication
					chunk[n].r = t0.r;
					chunk[n].i = t0.i;

					chunk[n + quarter].r = t1.r * w.r - t1.i * w.i;
					chunk[n + quarter].i = t1.r * w.i + t1.i * w.r;

					chunk[n + 2 * quarter].r = t2.r * w2.r - t2.i * w2.i;
					chunk[n + 2 * quarter].i = t2.r * w2.i + t2.i * w2.r;

					chunk[n + 3 * quarter].r = t3.r * w3.r - t3.i * w3.i;
					chunk[n + 3 * quarter].i = t3.r * w3.i + t3.i * w3.r;
				}
			}

			step >>= 2;
			omegaStep <<= 2;
		}
	}

	void rad4Itr::doRevTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		std::size_t step(4);
		std::size_t omegaStep(m_omegaSize / 4);

		while (step <= len) {
			// Perform the same pass as in the recursive case but over chunks of size step.
			for (Memory::SafePtr<Cplex> chunk(data); chunk < data + len; chunk += step) {
				std::size_t quarter(step >> 2);

				for (std::size_t k = 0; k < quarter; ++k) {
					Cplex w = m_omegaTable[k * omegaStep];
					Cplex w2 = m_omegaTable[2 * k * omegaStep];
					Cplex w3 = m_omegaTable[3 * k * omegaStep];

					w.i = -w.i;
					w2.i = -w2.i;
					w3.i = -w3.i;

					Cplex t0 = chunk[k];
					Cplex t1 = { chunk[k + quarter].r * w.r - chunk[k + quarter].i * w.i, chunk[k
						+ quarter].r * w.i + chunk[k + quarter].i * w.r };
					Cplex t2 = { chunk[k + 2 * quarter].r * w2.r - chunk[k + 2 * quarter].i * w2.i,
						chunk[k + 2 * quarter].r * w2.i + chunk[k + 2 * quarter].i * w2.r };
					Cplex t3 = { chunk[k + 3 * quarter].r * w3.r - chunk[k + 3 * quarter].i * w3.i,
						chunk[k + 3 * quarter].r * w3.i + chunk[k + 3 * quarter].i * w3.r };

					Cplex e0 = { t0.r + t2.r, t0.i + t2.i };
					Cplex e1 = { t0.r - t2.r, t0.i - t2.i };
					Cplex o0 = { t1.r + t3.r, t1.i + t3.i };
					Cplex o1 = { t1.r - t3.r, t1.i - t3.i };

					Cplex d0 = { e0.r + o0.r, e0.i + o0.i };
					Cplex d1 = { e1.r - o1.i, e1.i + o1.r }; // e1 + (+i)*o1
					Cplex d2 = { e0.r - o0.r, e0.i - o0.i };
					Cplex d3 = { e1.r + o1.i, e1.i - o1.r }; // e1 - (+i)*o1

					chunk[k] = d0;
					chunk[k + quarter] = d1;
					chunk[k + 2 * quarter] = d2;
					chunk[k + 3 * quarter] = d3;
				}
			}

			step <<= 2;
			omegaStep >>= 2;
		}
	}
}

