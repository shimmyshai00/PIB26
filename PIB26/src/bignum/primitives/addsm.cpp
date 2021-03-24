/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      addsm.cpp
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

#include "addsm.hpp"

namespace SDF::Bignum::Primitives
{
	unsigned int addSmall(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a,
		unsigned long smallNum, std::size_t len)
	{
		Digit carry(0);
		for(std::size_t i(0); i < len; ++i) {
			Digit tmp(a[i] + static_cast<Digit>(smallNum % BASE) + carry);
			if(tmp >= BASE) {
				tmp -= BASE;
				carry = 1;
			} else {
				carry = 0;
			}

			r[i] = tmp;
			smallNum /= BASE;
		}

		return smallNum + carry;
	}
}
