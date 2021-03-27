/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      rad2Itr.cpp
 * Timestamp: Mar 24, 2021
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

#include "rad2Itr.hpp"

#include <cmath>

namespace SDF::Bignum::Multiplication::Fft::Complex
{
	rad2Itr::rad2Itr(std::size_t maxFftSize)
		: m_maxFftSize(maxFftSize), m_omegaTable(maxFftSize)
	{
		// Precompute the roots in the omega table.
		for (std::size_t i(0); i < m_maxFftSize; ++i) {
			m_omegaTable[i].r = cos(-2.0 * M_PI * i / m_maxFftSize);
			m_omegaTable[i].i = sin(-2.0 * M_PI * i / m_maxFftSize);
		}
	}

	std::size_t rad2Itr::getMaxFftSize() const
	{
		return m_maxFftSize;
	}

	std::size_t rad2Itr::getNearestSafeLengthTo(std::size_t length) const
	{
		// Get the smallest power of two just larger than this length.
		std::size_t pow2Length(1);
		while (pow2Length < length) {
			pow2Length <<= 1;
		}

		return pow2Length;
	}

	void rad2Itr::doFwdTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		std::size_t step(len);
		std::size_t omegaStep(m_maxFftSize / len); // for exploiting that even though the table stores
																 // e^(-2piin/Nmax) for some Nmax, we have
																 // e^(-2piin/(Nmax/d)) = e^(-2pii(dn)/Nmax).

		while (step >= 2) {
			// Perform the same pass as in the recursive case but over chunks of size step.
			for (Memory::SafePtr<Cplex> chunk(data); chunk < data + len; chunk += step) {
				std::size_t half(step >> 1);

				for (std::size_t n = 0; n < half; ++n) {
					Cplex w = m_omegaTable[n * omegaStep];
					Cplex tmp1 = { chunk[n].r + chunk[n + half].r, chunk[n].i + chunk[n + half].i };
					Cplex tmp2 = { chunk[n].r - chunk[n + half].r, chunk[n].i - chunk[n + half].i };

					chunk[n].r = tmp1.r;
					chunk[n].i = tmp1.i;

					chunk[n + half].r = tmp2.r * w.r - tmp2.i * w.i;
					chunk[n + half].i = tmp2.r * w.i + tmp2.i * w.r;
				}
			}

			step >>= 1;
			omegaStep <<= 1;
		}
	}

	void rad2Itr::doRevTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		std::size_t step(2);
		std::size_t omegaStep(m_maxFftSize / 2);

		while (step <= len) {
			// Perform the same pass as in the recursive case but over chunks of size step.
			for (Memory::SafePtr<Cplex> chunk(data); chunk < data + len; chunk += step) {
				std::size_t half(step >> 1);

				for (std::size_t k = 0; k < half; ++k) {
					Cplex w = m_omegaTable[k * omegaStep];
					w.i = -w.i; // inverse roots are just conjugates

					Cplex tmp1 { chunk[k].r, chunk[k].i };
					Cplex tmp2 { chunk[k + half].r * w.r - chunk[k + half].i * w.i, chunk[k + half].r
						* w.i + chunk[k + half].i * w.r };

					chunk[k].r = tmp1.r + tmp2.r;
					chunk[k].i = tmp1.i + tmp2.i;

					chunk[k + half].r = tmp1.r - tmp2.r;
					chunk[k + half].i = tmp1.i - tmp2.i;
				}
			}

			step <<= 1;
			omegaStep >>= 1;
		}
	}
}
