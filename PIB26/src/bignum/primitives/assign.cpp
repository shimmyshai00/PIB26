/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      assign.cpp
 * Timestamp: Mar 16, 2021
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

#include "assign.hpp"

namespace SDF::Bignum::Primitives
{
	void zeroize(Memory::SafePtr<Digit> r, std::size_t len)
	{
		for (std::size_t i(0); i < len; ++i) {
			r[i] = 0;
		}
	}

	void nineize(Memory::SafePtr<Digit> r, std::size_t len)
	{
		for (std::size_t i(0); i < len; ++i) {
			r[i] = BASE - 1;
		}
	}

	void copy(Memory::SafePtr<Digit> dst, Memory::SafePtr<const Digit> src, std::size_t len)
	{
		for (std::size_t i(0); i < len; ++i) {
			dst[i] = src[i];
		}
	}

	std::size_t assignSmall(Memory::SafePtr<Digit> dst, unsigned int smallNum, std::size_t maxLen)
	{
		std::size_t i(0);
		for (; (i < maxLen) && smallNum; ++i, smallNum /= BASE) {
			dst[i] = smallNum % BASE;
		}

		return i;
	}
}
