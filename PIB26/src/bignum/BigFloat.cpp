/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      BigFloat.cpp
 * Timestamp: Mar 15, 2021
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

#include "BigFloat.hpp"

#include "../memory/buffers/local/RAMOnly.hpp"

#include "../util/printB26.hpp"

#include <sstream>
#include <iomanip>

#include <fstream>

namespace SDF::Bignum
{
	// Private constructor.
	BigFloat::BigFloat()
		: m_sign(SIGN_POSITIVE), m_exp(0), m_precNominal(1), m_totalLen(2 + GUARD_PREC)
	{
	}

	BigFloat::BigFloat(std::size_t size)
		: m_sign(SIGN_POSITIVE), m_exp(0), m_precNominal(size), m_totalLen(1 + size + GUARD_PREC), m_buffer(
			new Memory::Buffers::Local::RAMOnly<Digit>(1 + size + GUARD_PREC))
	{
		m_digits = m_buffer->accessData(0);
	}

	BigFloat::BigFloat(Memory::SafePtr<Digit> bufferPtr, std::size_t size)
		: m_sign(SIGN_POSITIVE), m_exp(0), m_precNominal(size), m_totalLen(1 + size + GUARD_PREC)
	{
		m_digits = bufferPtr;
	}

	std::size_t BigFloat::getPrecision() const
	{
		return m_precNominal;
	}

	std::size_t BigFloat::getBufferSize(std::size_t prec) {
		return 1 + prec + GUARD_PREC;
	}

	// Private member.
	BigFloat BigFloat::aliasTruncate(std::size_t truncPrec)
	{
		if (truncPrec > m_precNominal) {
			truncPrec = m_precNominal;
		}

		BigFloat rv;

		rv.m_sign = m_sign;
		rv.m_exp = m_exp;
		rv.m_digits = m_digits + (m_precNominal - truncPrec);
		rv.m_precNominal = truncPrec;
		rv.m_totalLen = 1 + truncPrec + GUARD_PREC;

		return rv;
	}

	// Private member
	void BigFloat::resize(std::size_t newPrec)
	{
		m_digits = m_digits + m_precNominal - newPrec;
		m_precNominal = newPrec;
		m_totalLen = 1 + newPrec + GUARD_PREC;
	}

	std::string BigFloat::print() const
	{
		std::stringstream ss;
		if (m_sign == SIGN_POSITIVE) {
			ss << "+";
		} else {
			ss << "-";
		}

		//ss << m_digits[m_precNominal + GUARD_PREC] << ".";
		ss << Util::printB26(m_digits[m_precNominal + GUARD_PREC], 0) << ".";

		for (std::ptrdiff_t i(m_precNominal + GUARD_PREC); i > 0; --i) {
			//ss << std::setfill('0') << std::setw(4) << m_digits[i - 1];
			ss << Util::printB26(m_digits[m_precNominal + GUARD_PREC], DIGS_PER_SMALL);
		}

		ss << " x BASE^" << m_exp;

		return ss.str();
	}

	void BigFloat::printNiceToFile(std::string fileName, std::size_t numDigits) const
	{
		std::ofstream ofile(fileName);

		// Figure out the position-from-MSD of the first nonzero small digit, when we take into
		// account the representation of the number in terms of large digits.
		std::size_t numLeadingZeroes(0);
		Digit firstDigit(m_digits[m_precNominal + GUARD_PREC]);
		while (firstDigit < BASE / BASE_MINOR) {
			firstDigit *= BASE_MINOR;
			++numLeadingZeroes;
		}

		// Position from MSD, not LSD
		std::size_t smallDigitPos(numLeadingZeroes);

		ofile << Util::printB26Digit(getSmallDigit(smallDigitPos)) << "." << std::endl;

		// Now format the rest.
		for (std::size_t i(1); i <= numDigits; ++i) {
			ofile << Util::printB26Digit(getSmallDigit(smallDigitPos + i));

			if (i % 10 == 0) {
				ofile << " ";
			}

			if (i % 50 == 0) {
				ofile << " : " << i << std::endl;
			}

			if (i % 1000 == 0) {
				ofile << std::endl;
			}
		}

		ofile << std::endl;
	}

	// Private member.
	Digit BigFloat::getSmallDigit(std::size_t which) const
	{
		std::size_t bigDigitPosFromMsd(which / DIGS_PER_DIG);
		std::size_t shift(DIGS_PER_DIG - 1 - (which % DIGS_PER_DIG));
		Digit bigDigit(m_digits[m_precNominal + GUARD_PREC - bigDigitPosFromMsd]);

		while (shift) {
			bigDigit /= BASE_MINOR;
			--shift;
		}

		return bigDigit % BASE_MINOR;
	}
}
