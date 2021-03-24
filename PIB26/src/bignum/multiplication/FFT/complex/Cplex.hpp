/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      Cplex.hpp
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

#ifndef SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_CPLEX_HPP_
#define SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_CPLEX_HPP_

namespace SDF::Bignum::Multiplication::Fft::Complex
{
	// A struct to hold a complex number. We don't use std::complex as we don't trust it to be fast
	// enough.
	struct Cplex
	{
			double r, i;

			Cplex()
				: r(0.0), i(0.0)
			{
			}

			Cplex(double r_, double i_)
				: r(r_), i(i_)
			{
			}

			Cplex& operator=(double r)
			{
				this->r = r;
				this->i = 0.0;
				return *this;
			}
	};
}

#endif /* SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_CPLEX_HPP_ */
