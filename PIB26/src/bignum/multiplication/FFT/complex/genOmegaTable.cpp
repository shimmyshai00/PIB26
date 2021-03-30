/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      genOmegaTable.cpp
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

#include "genOmegaTable.hpp"

#include "../../../../memory/buffers/local/RAMOnly.hpp"

#include <cmath>

namespace SDF::Bignum::Multiplication::Fft::Complex {
	std::unique_ptr<Memory::ILocalBuffer<Cplex>> genOmegaTable(std::size_t length) {
		std::unique_ptr<Memory::ILocalBuffer<Cplex>> buf(new Memory::Buffers::Local::RAMOnly<Cplex>(length));
		Memory::SafePtr<Cplex> bufPtr(buf->accessData(0));

		for(std::size_t i(0); i < length; ++i) {
			bufPtr[i].r = cos(-2.0 * M_PI * i / length);
			bufPtr[i].i = sin(-2.0 * M_PI * i / length);
		}

		return buf;
	}
}

