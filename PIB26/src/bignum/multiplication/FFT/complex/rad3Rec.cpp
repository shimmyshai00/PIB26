/*
 * PIB26 version 0.0.1
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

#include <cmath>

namespace SDF::Bignum::Multiplication::Fft::Complex {
	rad3Rec::rad3Rec()
	{
	}

	std::size_t rad3Rec::getNearestSafeLengthTo(std::size_t length) const
	{
		// Get the smallest power of two just larger than this length OR the smallest power-of-
		// 2 times 3 just larger, whichever one is closer.
		std::size_t testLength(1);
		std::size_t mulStage(0);
		while(testLength < length) {
			// Build a sequence of the form 1, 2, 3, 2*2, 2*3, 2*2*2, 2*2*3, 2*2*2*2, ...
			if(mulStage == 0) {
				testLength <<= 1;
				mulStage = 1;
			} else if(mulStage == 1) {
				testLength = (testLength >> 1) * 3;
				mulStage = 2;
			} else if(mulStage == 2) {
				testLength = (testLength / 3) << 2;
				mulStage = 1;
			}
		}

		return testLength;
	}

	void rad3Rec::doFwdTransform(Memory::SafePtr<Cplex> data, std::size_t len)
	{
		if (len % 3 != 0) {
			// Switch to radix-2 transform.
			m_rad2Fft.doFwdTransform(data, len);
		} else {
			std::size_t third(len / 3);
			std::size_t full(len);

			double w0Cos = sin(-M_PI / full);
			w0Cos = 2.0 * w0Cos * w0Cos;
			double w0Sin = sin(-2 * M_PI / full);

			double w0SqCos = sin(-2 * M_PI / full);
			w0SqCos = 2.0 * w0SqCos * w0SqCos;
			double w0SqSin = sin(-4 * M_PI / full);

			Cplex w { 1, 0 };
			Cplex w2 { 1, 0 }; // w^2

			for (std::size_t n = 0; n < third; ++n) {
				static const double c_sq3o2 = 0.8660254037844386l; // sqrt(3)/2
				Cplex tmp1, tmp2, tmp3;

				// Tip: generate these with a CAS; not by hand!
				tmp1.r = data[n].r + data[n + third].r + data[n + 2*third].r;
				tmp1.i = data[n].i + data[n + third].i + data[n + 2*third].i;

				tmp2.r = data[n].r - 0.5 * (data[n + third].r + data[n + 2*third].r)
					                + c_sq3o2 * (data[n + third].i - data[n + 2*third].i);
				tmp2.i = data[n].i - 0.5 * (data[n + third].i + data[n + 2*third].i)
					                - c_sq3o2 * (data[n + third].r - data[n + 2*third].r);

				tmp3.r = data[n].r - 0.5 * (data[n + third].r + data[n + 2*third].r)
					                - c_sq3o2 * (data[n + third].i - data[n + 2*third].i);
				tmp3.i = data[n].i - 0.5 * (data[n + third].i + data[n + 2*third].i)
									    + c_sq3o2 * (data[n + third].r - data[n + 2*third].r);

				data[n].r = tmp1.r;
				data[n].i = tmp1.i;

				data[n + third].r = tmp2.r * w.r - tmp2.i * w.i;
				data[n + third].i = tmp2.r * w.i + tmp2.i * w.r;

				data[n + 2*third].r = tmp3.r * w2.r - tmp3.i * w2.i;
				data[n + 2*third].i = tmp3.r * w2.i + tmp3.i * w2.r;

				if (n % 1000 == 0) {
					w.r = cos(-2.0 * M_PI * (n+1) / full);
					w.i = sin(-2.0 * M_PI * (n+1) / full);

					w2.r = cos(-4.0 * M_PI * (n+1) / full);
					w2.i = sin(-4.0 * M_PI * (n+1) / full);
				} else {
					tmp1.r = w.r - (w.r * w0Cos + w.i * w0Sin);
					tmp1.i = w.i - (-w.r * w0Sin + w.i * w0Cos);

					w.r = tmp1.r;
					w.i = tmp1.i;

					tmp1.r = w2.r - (w2.r * w0SqCos + w2.i * w0SqSin);
					tmp1.i = w2.i - (-w2.r * w0SqSin + w2.i * w0SqCos);

					w2.r = tmp1.r;
					w2.i = tmp1.i;
				}
			}

			doFwdTransform(data, third);
			doFwdTransform(data + third, third);
			doFwdTransform(data + 2*third, third);
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
			doRevTransform(data + 2*third, third);

			double w0Cos = sin(M_PI / full);
			w0Cos = 2.0 * w0Cos * w0Cos;
			double w0Sin = sin(2 * M_PI / full);

			double w0SqCos = sin(2 * M_PI / full);
			w0SqCos = 2.0 * w0SqCos * w0SqCos;
			double w0SqSin = sin(4 * M_PI / full);

			Cplex w { 1, 0 };
			Cplex w2 { 1, 0 }; // w^2

			for (std::size_t n = 0; n < third; ++n) {
				static const double c_sq3o2 = 0.8660254037844386l; // sqrt(3)/2
				Cplex tmp1, tmp2, tmp3;

				tmp1.r = data[n].r;
				tmp1.i = data[n].i;

				tmp2.r = data[n + third].r * w.r - data[n + third].i * w.i;
				tmp2.i = data[n + third].r * w.i + data[n + third].i * w.r;

				tmp3.r = data[n + 2*third].r * w2.r - data[n + 2*third].i * w2.i;
				tmp3.i = data[n + 2*third].r * w2.i + data[n + 2*third].i * w2.r;

				data[n].r = tmp1.r + tmp2.r + tmp3.r;
				data[n].i = tmp1.i + tmp2.i + tmp3.i;

				// n.b. some of the signs change here due to the conjugated trig
				data[n + third].r = tmp1.r - 0.5 * (tmp2.r + tmp3.r) - c_sq3o2 * (tmp2.i - tmp3.i);
				data[n + third].i = tmp1.i - 0.5 * (tmp2.i + tmp3.i) + c_sq3o2 * (tmp2.r - tmp3.r);

				data[n + 2*third].r = tmp1.r - 0.5 * (tmp2.r + tmp3.r) + c_sq3o2 * (tmp2.i - tmp3.i);
				data[n + 2*third].i = tmp1.i - 0.5 * (tmp2.i + tmp3.i) - c_sq3o2 * (tmp2.r - tmp3.r);

				if (n % 1000 == 0) {
					w.r = cos(2.0 * M_PI * (n+1) / full);
					w.i = sin(2.0 * M_PI * (n+1) / full);

					w2.r = cos(4.0 * M_PI * (n+1) / full);
					w2.i = sin(4.0 * M_PI * (n+1) / full);
				} else {
					tmp1.r = w.r - (w.r * w0Cos + w.i * w0Sin);
					tmp1.i = w.i - (-w.r * w0Sin + w.i * w0Cos);

					w.r = tmp1.r;
					w.i = tmp1.i;

					tmp1.r = w2.r - (w2.r * w0SqCos + w2.i * w0SqSin);
					tmp1.i = w2.i - (-w2.r * w0SqSin + w2.i * w0SqCos);

					w2.r = tmp1.r;
					w2.i = tmp1.i;
				}
			}
		}
	}
}
