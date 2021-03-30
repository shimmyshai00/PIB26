/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      shift.cpp
 * Timestamp: Mar 17, 2021
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

#include "shift.hpp"

#include "assign.hpp"

namespace SDF::Bignum::Primitives
{
	void lshift(Memory::SafePtr<Digit> r, std::size_t shift, std::size_t len)
	{
		for (std::size_t i(len + shift); i > shift; --i) {
			r[i - 1] = r[i - 1 - shift];
		}

		for (std::size_t i(shift); i > 0; --i) {
			r[i - 1] = 0;
		}
	}

	void rshift(Memory::SafePtr<Digit> r, std::size_t shift, std::size_t len)
	{
		if (shift >= len) {
			// r completely shifted away.
			zeroize(r, len);
		} else {
			for (std::size_t i(0); i < len - shift; ++i) {
				r[i] = r[i + shift];
			}

			for (std::size_t i(len - shift); i < len; ++i) {
				r[i] = 0;
			}
		}
	}
}
