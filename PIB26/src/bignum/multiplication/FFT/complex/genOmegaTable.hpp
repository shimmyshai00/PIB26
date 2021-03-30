/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      genOmegaTable.hpp
 * Timestamp: Mar 28, 2021
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
#ifndef SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_GENOMEGATABLE_HPP_
#define SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_GENOMEGATABLE_HPP_

#include "../../../../memory/ILocalBuffer.hpp"

#include "Cplex.hpp"

#include <cstddef>
#include <memory>

namespace SDF::Bignum::Multiplication::Fft::Complex {
	// Function:   genOmegaTable
	// Purpose:    Generate the complex roots of unity (omega) table for FFT sizes up to a given
	//             size.
	// Parameters: length - The length of table to generate.
	// Returns:    The newly-generated omega table.
	std::unique_ptr<Memory::ILocalBuffer<Cplex>> genOmegaTable(std::size_t length);
}

#endif /* SRC_BIGNUM_MULTIPLICATION_FFT_COMPLEX_GENOMEGATABLE_HPP_ */
