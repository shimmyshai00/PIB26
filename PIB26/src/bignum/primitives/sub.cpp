/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      sub.cpp
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

#include "sub.hpp"

namespace SDF::Bignum::Primitives
{
	Digit sub(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a,
		Memory::SafePtr<const Digit> b, std::size_t len, Digit borrow)
	{
		for (std::size_t i(0); i < len; ++i) {
			Digit tmp(a[i] - b[i] - borrow);
			if (tmp < 0) {
				tmp += BASE;
				borrow = 1;
			} else {
				borrow = 0;
			}

			r[i] = tmp;
		}

		return borrow;
	}

	Digit propagateBorrow(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a, Digit borrow,
		std::size_t len)
	{
		for (std::size_t i(0); i < len; ++i) {
			Digit tmp(a[i] - borrow);
			if (tmp < 0) {
				tmp += BASE;
				borrow = 1;
			} else {
				borrow = 0;
			}

			r[i] = tmp;
		}

		return borrow;
	}

	Digit neg(Memory::SafePtr<Digit> r, Memory::SafePtr<const Digit> a, Digit borrow,
		std::size_t len)
	{
		for (std::size_t i(0); i < len; ++i) {
			Digit tmp(0 - a[i] - borrow);
			if (tmp < 0) {
				tmp += BASE;
				borrow = 1;
			} else {
				borrow = 0;
			}

			r[i] = tmp;
		}

		return borrow;
	}

	void propagateSub(Memory::SafePtr<Digit> r, std::size_t rLen, Memory::SafePtr<const Digit> a,
		std::size_t aLen)
	{
		if (rLen < aLen) {
			Digit borrow(0);
			for (std::size_t i(0); i < rLen; ++i) {
				r[i] -= a[i] + borrow;
				if (r[i] < 0) {
					r[i] += BASE;
					borrow = 1;
				} else {
					borrow = 0;
				}
			}
		} else {
			Digit borrow(0);
			for (std::size_t i(0); i < aLen; ++i) {
				r[i] -= a[i] + borrow;
				if (r[i] < 0) {
					r[i] += BASE;
					borrow = 1;
				} else {
					borrow = 0;
				}
			}

			for (std::size_t i(aLen); i < rLen; ++i) {
				r[i] -= borrow;
				if (r[i] < 0) {
					r[i] += BASE;
					borrow = 1;
				} else {
					borrow = 0;
				}
			}
		}
	}
}
