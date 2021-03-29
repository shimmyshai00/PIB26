/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      BigInt.cpp
 * Timestamp: Mar 13, 2021
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

#include "BigInt.hpp"

#include "../memory/buffers/local/RAMOnly.hpp"

#include "../util/printB26.hpp"

#include <sstream>
#include <iomanip>

namespace SDF::Bignum
{
	BigInt::BigInt(std::size_t size)
		: m_sign(SIGN_POSITIVE), m_digitsAlloc(size), m_digitsUsed(size), m_buffer(
			new Memory::Buffers::Local::RAMOnly<Digit>(size)), m_digits(m_buffer->accessData(0))
	{
	}

	BigInt::BigInt(Memory::SafePtr<Digit> safePtr, std::size_t size)
		: m_sign(SIGN_POSITIVE), m_digitsAlloc(size), m_digitsUsed(size), m_digits(safePtr)
	{
	}

	BigInt::~BigInt()
	{
	}

	std::string BigInt::print() const
	{
		if (m_digitsUsed == 0) {
			//return "0";
			return "A";
		} else {
			std::stringstream ss;
			if (m_sign == SIGN_POSITIVE) {
				ss << "+";
			} else {
				ss << "-";
			}

			//ss << m_digits[m_digitsUsed - 1];
			ss << Util::printB26(m_digits[m_digitsUsed - 1], 0);

			for (std::size_t i(m_digitsUsed - 1); i > 0; --i) {
				//ss << std::setfill('0') << std::setw(4) << m_digits[i - 1];
				ss << Util::printB26(m_digits[m_digitsUsed - 1], DIGS_PER_SMALL);
			}

			return ss.str();
		}
	}

	std::size_t BigInt::getDgsAlloc() const
	{
		return m_digitsAlloc;
	}

	std::size_t BigInt::getDgsUsed() const
	{
		return m_digitsUsed;
	}
}
