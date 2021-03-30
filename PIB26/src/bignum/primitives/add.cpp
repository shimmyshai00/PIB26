/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      add.cpp
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

#include "add.hpp"

namespace SDF::Bignum::Primitives
{
	Digit add(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a,
		Memory::SafePtr<const Digit> b, std::size_t len, Digit carry)
	{
		for (std::size_t i(0); i < len; ++i) {
			Digit tmp(a[i] + b[i] + carry);
			if (tmp >= BASE) {
				tmp -= BASE;
				carry = 1;
			} else {
				carry = 0;
			}

			r[i] = tmp;
		}

		return carry;
	}

	Digit propagateCarry(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a, Digit carry,
		std::size_t len)
	{
		for (std::size_t i(0); i < len; ++i) {
			Digit tmp(a[i] + carry);
			if (tmp == BASE) {
				tmp = 0;
				carry = 1;
			} else {
				carry = 0;
			}

			r[i] = tmp;
		}

		return carry;
	}

	Digit unevenAdd(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a, std::size_t aLen,
		Memory::SafePtr<const Digit> b, std::size_t bLen, Digit carry)
	{
		if (aLen >= bLen) {
			for (std::size_t i(0); i < bLen; ++i) {
				Digit tmp(a[i] + b[i] + carry);
				if (tmp >= BASE) {
					tmp -= BASE;
					carry = 1;
				} else {
					carry = 0;
				}

				r[i] = tmp;
			}

			for (std::size_t i(bLen); i < aLen; ++i) {
				r[i] = a[i] + carry;
				if (r[i] == BASE) {
					r[i] = 0;
					carry = 1;
				} else {
					carry = 0;
				}
			}
		} else {
			for (std::size_t i(0); i < aLen; ++i) {
				Digit tmp(a[i] + b[i] + carry);
				if (tmp >= BASE) {
					tmp -= BASE;
					carry = 1;
				} else {
					carry = 0;
				}

				r[i] = tmp;
			}

			for (std::size_t i(aLen); i < bLen; ++i) {
				r[i] = b[i] + carry;
				if (r[i] == BASE) {
					r[i] = 0;
					carry = 1;
				} else {
					carry = 0;
				}
			}
		}

		return carry;
	}

	void propagateAdd(Memory::SafePtr<Digit> r, std::size_t rLen, Memory::SafePtr<const Digit> a,
		std::size_t aLen)
	{
		if (rLen < aLen) {
			Digit carry(0);
			for (std::size_t i(0); i < rLen; ++i) {
				r[i] += a[i] + carry;
				if (r[i] >= BASE) {
					r[i] -= BASE;
					carry = 1;
				} else {
					carry = 0;
				}
			}
		} else {
			Digit carry(0);
			for (std::size_t i(0); i < aLen; ++i) {
				r[i] += a[i] + carry;
				if (r[i] >= BASE) {
					r[i] -= BASE;
					carry = 1;
				} else {
					carry = 0;
				}
			}

			for (std::size_t i(aLen); i < rLen; ++i) {
				r[i] += carry;
				if (r[i] == BASE) {
					r[i] = 0;
					carry = 1;
				} else {
					carry = 0;
				}
			}
		}
	}
}
