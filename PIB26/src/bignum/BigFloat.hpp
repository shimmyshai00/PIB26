/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      BigFloat.hpp
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

#ifndef SRC_BIGNUM_BIGFLOAT_HPP_
#define SRC_BIGNUM_BIGFLOAT_HPP_

#include "defs.hpp"

#include "../memory/ILocalBuffer.hpp"
#include "../memory/SafePtr.hpp"

#include "../util/ITicker.hpp"

#include <cstddef>
#include <string>
#include <memory>

namespace SDF::Bignum
{
	class IMultiplicationStrategy;
	class BigFloat;
}

namespace SDF::Newton
{
	void recip(Bignum::BigFloat &r, Bignum::BigFloat &a, Bignum::IMultiplicationStrategy &strategy);
}

namespace SDF::Bignum
{
	class IMultiplicationStrategy;
	class BigInt;

	// Class:      BigFloat
	// Purpose:    Provides a big floating point bignum type. Note: unlike BigInts, BigFloats
	//             have manually-adjusted precision and do not adjust their precision
	//             automatically.
	// Parameters: None.
	class BigFloat
	{
		public:
			// Function:   BigFloat
			// Purpose:    Construct a new big float with its own buffer and of a given length.
			// Parameters: size - The number of digits in the BigFloat.
			BigFloat(std::size_t size);

			// Function:   BigFloat
			// Purpose:    Construct a new big float on top of an existing buffer.
			// Parameters: bufferPtr - The pointer into the buffer to construct on.
			//             size - The number of digits in the BigFloat.
			BigFloat(Memory::SafePtr<Digit> bufferPtr, std::size_t size);

			// Function:   getPrecision
			// Purpose:    Get the precision of this BigFloat in digits. This is the amount of
			//             guaranteed precision - the BigFloat may have slightly more becaue it stores
			//             this much plus one full size digit.
			// Parameters: None.
			// Returns:    The number of digits of precision the BigFloat has.
			std::size_t getPrecision() const;

			// Function:   getBufferSize
			// Purpose:    Get the needed buffer size for a given precision.
			// Parameters: prec - The precision to inquire about.
			// Returns:    The needed buffer size in digits.
			static std::size_t getBufferSize(std::size_t prec);

			// Function:   print
			// Purpose:    Prints the value of the BigFloat in base-BASE_MINOR format.
			// Parmeters:  None.
			// Returns:    The string printed value of the BigFloat.
			std::string print() const;

			// Function:   printNiceToFile
			// Purpose:    Prints a nicely-formatted version of this BigFloat to a file
			//             with the specified number of base-BASE_MINOR digits (not base-BASE digits).
			// Parameters: fileName - The name of the file to print to
			//             numDigits - The number of digits to print.
			// Returns:    None.
			void printNiceToFile(std::string fileName, std::size_t numDigits) const;

			// Function:   assign
			// Purpose:    Assign a small integer value to this BigFloat.
			// Parameters: smallNum - The small number to assign.
			// Returns:    None.
			void assign(int smallNum);
			void assign(unsigned int smallNum);

			// Function:   assign
			// Purpose:    Assign another BigFloat to this BigFloat.
			// Parameters: rhs - The BigFloat to assign.
			// Returns:    None.
			void assign(const BigFloat &rhs);

			// Function:   assign
			// Purpose:    Assign a BigInt to this BigFloat.
			// Parameters: rhs - The BigInt to assign.
			// Returns:    None.
			void assign(const BigInt &rhs);

			// Function:   compare
			// Purpose:    Spaceship operator - compare the values of two BigFloats.
			// Parameters: rhs - The other BigFloat to compare this one to.
			// Returns:    +1 if this > rhs
			//              0 if this == rhs
			//             -1 if this < rhs.
			int compare(const BigFloat &rhs) const;

			// Function:   compare
			// Purpose:    Compare this BigFloat to a small number.
			// Parameters: smallNum - The small number to compare to.
			// Returns:    +1 if this > smallNum
			//              0 if this == smallNum
			//             -1 if this < smallNum
			int compare(int smallNum) const;
			int compare(unsigned int smallNum) const;

			// Function:   add
			// Purpose:    Add two BigFloats together, storing the result in this one. This method
			//             should only be used for out-of-place additions.
			// Parameters: num1 - The first operand to add.
			//             num2 - The second operand to add.
			// Returns:    None.
			void add(const BigFloat &num1, const BigFloat &num2);

			// Function:   addIp
			// Purpose:    Perform in-place addition of a BigFloat to this one.
			// Parameters: num - the BigFloat to add to this one.
			// Returns:    None.
			void addIp(const BigFloat &num1);

			// Function:   sub
			// Purpose:    Sub one BigFloat from another, storing the result in this one. This method
			//             should only be used for out-of-place subtractions.
			// Parameters: num1 - The first operand to add.
			//             num2 - The second operand to add.
			// Returns:    None.
			void sub(const BigFloat &num1, const BigFloat &num2);

			// Function:   subIp
			// Purpose:    Perform in-place subtraction of a BigFloat from this one.
			// Parameters: num - the BigFloat to sub from this one.
			// Returns:    None.
			void subIp(const BigFloat &num1);

			// Function:   mul
			// Purpose:    Multiplies two BigFloats together. Note: multiplication always uses a
			//             separate buffer, so it will also work in-place.
			// Parameters: num1 - The first operand to mul.
			//             num2 - The second operand to mul.
			//             strategy - The multiplication strategy (algorithm) to use.
			// Returns:    None.
			void mul(const BigFloat &num1, const BigFloat &num2, IMultiplicationStrategy &strategy);

			// Function:   mul
			// Purpose:    Multiplies a BigFloat by a BigInt. Note: multiplication always uses a
			//             separate buffer, so it will also work in-place.
			// Parameters: num1 - The first operand to mul.
			//             num2 - The BigInt to multiply by.
			//             strategy - The multiplication strategy (algorithm) to use.
			// Returns:    None.
			void mul(const BigFloat &num1, const BigInt &num2, IMultiplicationStrategy &strategy);

			// Function:   mul
			// Purpose:    Multiplies a BigFloat by a small number.
			// Parameters: num1 - The BigFloat to mul.
			//             smallNum - The small number to mul by.
			// Returns:    None.
			void mul(const BigFloat &num1, int smallNum);
			void mul(const BigFloat &num1, unsigned int smallNum);

			// Function:   mulIp
			// Purpose:    In-place multiplication of a BigFloat by a small number.
			// Parameters: smallNum - The small number to multiply by.
			// Returns:    None.
			void mulIp(int smallNum);
			void mulIp(unsigned int smallNum);

			// Function:   div
			// Purpose:    Divide a BigFloat by a small number.
			// Parameters: num1 - The BigFloat to divide.
			//             smallNum - The small number to divide by.
			// Returns:    None.
			void div(const BigFloat &num1, int smallNum);
			void div(const BigFloat &num1, unsigned int smallNum);

			// Function:   div
			// Purpose:    Divide a BigFloat by a small number.
			// Parameters: smallNum - The small number to divide by.
			// Returns:    None.
			void divIp(int smallNum);
			void divIp(unsigned int smallNum);

			// Function:  recip
			// Purpose:   Compute the reciprocal of a BigFloat using the Newton method.
			// Arguments: a - The BigFloat to take the reciprocal of.
			//            strategy - The multiplication strategy to use.
			//            ticker - A progress ticker to show the progress of this long-running
			//                     operation.
			// Returns:   None.
			void recip(BigFloat &a, Bignum::IMultiplicationStrategy &strategy, Util::ITicker *ticker);

			// Function:  invsqrt
			// Purpose:   Compute the reciprocal square root of a BigFloat using the Newton method.
			// Arguments: a - The BigFloat to take the inv square root of.
			//            strategy - The multiplication strategy to use.
			//            ticker - A progress ticker to show the progress of this long-running
			//                     operation.
			// Returns:   None.
			void invsqrt(BigFloat &a, Bignum::IMultiplicationStrategy &strategy,
				Util::ITicker *ticker);

			// Function:  invsqrt
			// Purpose:   Compute the reciprocal square root of a small number using the Newton method.
			// Arguments: a - The small number to take the inv square root of.
			//            strategy - The multiplication strategy to use.
			//            ticker - A progress ticker to show the progress of this long-running
			//                     operation.
			// Returns:   None.
			void invsqrt(unsigned int a, Bignum::IMultiplicationStrategy &strategy,
				Util::ITicker *ticker);
		private:
			// Declares how much extra "slop" precision to keep around to buffer rounding errors.
			static const std::size_t GUARD_PREC = 1;

			Sign m_sign;
			std::ptrdiff_t m_exp;

			std::size_t m_precNominal; // The nominal precision of the BigFloat's fraction.
			std::size_t m_totalLen; // The total length of the BigFloat
			                        // (equals m_precNominal + 1 + GUARD_PREC).
			std::unique_ptr<Memory::ILocalBuffer<Digit>> m_buffer;
			Memory::SafePtr<Digit> m_digits; // This always stores m_totalLen digits.

			BigFloat();

			// Function:   getSmallDigit
			// Purpose:    Get a small (base-BASE_MINOR) digit of this number.
			// Parameters: which - Which one to get
			// Returns:    The small digit.
			Digit getSmallDigit(std::size_t which) const;

			// Function:   aliasTruncate
			// Purpose:    Create a truncated version of this BigFloat on the same buffer.
			// Parameters: truncPrec - The precision to truncate to.
			// Returns:    The alias BigFloat.
			BigFloat aliasTruncate(std::size_t truncPrec);

			// Function:   resize
			// Purpose:    Resizes this BigFloat itself, changing the buffer pointers while maintaining
			//             the most-significant digit. NOTE: this operation does not save the original
			//             pointer location!
			// Parameters: newPrec - The precision to reset to.
			// Returns:    None.
			void resize(std::size_t newPrec);

			// Unsigned implementation methods.
			void uassign(unsigned int smallNum);
			void uassign(const BigFloat &rhs);
			void uassign(const BigInt &rhs);

			int ucompare(const BigFloat &rhs) const;
			int ucompare(unsigned int smallNum) const;

			void uadd(const BigFloat &num1, const BigFloat &num2);
			void uaddIp(const BigFloat &num);

			void usub(const BigFloat &num1, const BigFloat &num2);
			void usubIp(const BigFloat &num);

			void umul(const BigFloat &num1, const BigFloat &num2, IMultiplicationStrategy &strategy);
			void umul(const BigFloat &num1, const BigInt &num2, IMultiplicationStrategy &strategy);
			void umul(const BigFloat &num1, unsigned int smallNum);

			void umulIp(unsigned int smallNum);

			void udiv(const BigFloat &num1, unsigned int smallNum);
			void udivIp(unsigned int smallNum);
	};
}

#endif /* SRC_BIGNUM_BIGFLOAT_HPP_ */
