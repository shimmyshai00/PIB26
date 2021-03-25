/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      FFTTweak.hpp
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

#ifndef SRC_BIGNUM_MULTIPLICATION_FFT_FFTTWEAK_HPP_
#define SRC_BIGNUM_MULTIPLICATION_FFT_FFTTWEAK_HPP_

#include <cstddef>

namespace SDF::Bignum::Multiplication::Fft {
	// Defines tweak constants for the FFT. Ideally (TBA!) we should have the program assess these
	// parameters from the specs of the system we are running on.
	static const std::size_t g_cacheSize = 262144; // The size of the CPU L1 cache in bytes.
}

#endif /* SRC_BIGNUM_MULTIPLICATION_FFT_FFTTWEAK_HPP_ */
