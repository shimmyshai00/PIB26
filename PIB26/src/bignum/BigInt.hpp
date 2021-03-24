/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      BigInt.hpp
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

#ifndef SRC_BIGNUM_BIGINT_HPP_
#define SRC_BIGNUM_BIGINT_HPP_

#include "defs.hpp"
#include "../memory/ILocalBuffer.hpp"
#include "../memory/SafePtr.hpp"

#include <cstddef>
#include <string>
#include <memory>

namespace SDF::Bignum
{
	class IMultiplicationStrategy;
	class BigFloat;

	// Class:      BigInt
	// Purpose:    Provides a big integer bignum type.
	// Parameters: None.
	class BigInt
	{
		public:
			// Function:   BigInt
			// Purpose:    Construct a new big integer with its own buffer and of a given length.
			// Parameters: size - The size of the new bignum to construct, in digits.
			BigInt(std::size_t size);

			// Function:   BigInt
			// Purpose:    Construct a new big integer on top of an existing buffer.
			// Parameters: safePtr - The pointer to the location in the buffer of the BigInt's LSD.
			//             size - The size of the BigInt to construct.
			BigInt(Memory::SafePtr<Digit> safePtr, std::size_t size);

			~BigInt();

			// Function:   getDgsAlloc
			// Purpose:    Get the number of digits allocated to this bignum.
			// Parameters: None.
			// Returns:    The number of allocated digits.
			std::size_t getDgsAlloc() const;

			// Function:   getDgsUsed
			// Purpose:    Get the number of digits used by this bignum.
			// Parameters: None.
			// Returns:    The number of used digits.
			std::size_t getDgsUsed() const;

			// Function:   print
			// Purpose:    Prints the value of the BigInt in base-BASE_MINOR format.
			// Parmeters:  None.
			// Returns:    The string printed value of the BigInt.
			std::string print() const;

			// Function:   assign
			// Purpose:    Assign a small integer value to this BigInt.
			// Parameters: smallNum - The small number to assign.
			// Returns:    None.
			void assign(int smallNum);
			void assign(unsigned int smallNum);

			// Function:   assign
			// Purpose:    Assign another BigInt to this BigInt.
			// Parameters: rhs - The BigInt to assign.
			// Returns:    None.
			void assign(const BigInt &rhs);

			// Function:   compare
			// Purpose:    Spaceship operator - compare the values of two BigInts.
			// Parameters: rhs - The other BigInt to compare this one to.
			// Returns:    +1 if this > rhs
			//              0 if this == rhs
			//             -1 if this < rhs.
			int compare(const BigInt &rhs) const;

			// Function:   compare
			// Purpose:    Compare this BigInt to a small number.
			// Parameters: smallNum - The small number to compare to.
			// Returns:    +1 if this > smallNum
			//              0 if this == smallNum
			//             -1 if this < smallNum
			int compare(int smallNum) const;
			int compare(unsigned int smallNum) const;

			// Function:   add
			// Purpose:    Add two BigInts together, storing the result in this one. This method
			//             should only be used for out-of-place additions.
			// Parameters: num1 - The first operand to add.
			//             num2 - The second operand to add.
			// Returns:    None.
			void add(const BigInt &num1, const BigInt &num2);

			// Function:   addIp
			// Purpose:    Perform in-place addition of a BigInt to this one.
			// Parameters: num - the BigInt to add to this one.
			// Returns:    None.
			void addIp(const BigInt &num1);


			// Function:   sub
			// Purpose:    Sub one BigInt from another, storing the result in this one. This method
			//             should only be used for out-of-place subtractions.
			// Parameters: num1 - The first operand to add.
			//             num2 - The second operand to add.
			// Returns:    None.
			void sub(const BigInt &num1, const BigInt &num2);

			// Function:   subIp
			// Purpose:    Perform in-place subtraction of a BigInt from this one.
			// Parameters: num - the BigInt to sub from this one.
			// Returns:    None.
			void subIp(const BigInt &num1);

			// Function:   neg
			// Purpose:    Negate this BigInt.
			// Parameters: None.
			// Returns:    None.
			void neg();

			// Function:   mul
			// Purpose:    Multiplies two BigInts together.  Note: multiplication always uses a
			//             separate buffer, so it will also work in-place.
			// Parameters: num1 - The first operand to mul.
			//             num2 - The second operand to mul.
			//             strategy - The multiplication strategy (algorithm) to use.
			// Returns:    None.
			void mul(const BigInt &num1, const BigInt &num2, IMultiplicationStrategy &strategy);

			// Function:   mul
			// Purpose:    Multiplies a BigInt by a small number.
			// Parameters: num1 - The BigInt to mul.
			//             smallNum - The small number to mul by.
			// Returns:    None.
			void mul(const BigInt &num1, int smallNum);
			void mul(const BigInt &num1, unsigned int smallNum);

			// Function:   mulIp
			// Purpose:    In-place multiplication of a BigInt by a small number.
			// Parameters: smallNum - The small number to multiply by.
			// Returns:    None.
			void mulIp(int smallNum);
			void mulIp(unsigned int smallNum);

			// Function:   div
			// Purpose:    Divide a BigInt by a small number.
			// Parameters: num1 - The BigInt to divide.
			//             smallNum - The small number to divide by.
			// Returns:    None.
			void div(const BigInt &num1, int smallNum);
			void div(const BigInt &num1, unsigned int smallNum);

			// Function:   div
			// Purpose:    Divide a BigInt by a small number.
			// Parameters: smallNum - The small number to divide by.
			// Returns:    None.
			void divIp(int smallNum);
			void divIp(unsigned int smallNum);
		public:
			Sign m_sign;

			std::size_t m_digitsAlloc;
			std::size_t m_digitsUsed;

			std::unique_ptr<Memory::ILocalBuffer<Digit>> m_buffer;
			Memory::SafePtr<Digit> m_digits;
		private:

			friend class BigFloat;

			// Unsigned implementation methods.
			void uassign(unsigned int smallNum);
			void uassign(const BigInt &rhs);

			int ucompare(const BigInt &rhs) const;
			int ucompare(unsigned int smallNum) const;

			void uadd(const BigInt &num1, const BigInt &num2);
			void uaddIp(const BigInt &num);

			void usub(const BigInt &num1, const BigInt &num2);
			void usubIp(const BigInt &num);

			void umul(const BigInt &num1, const BigInt &num2, IMultiplicationStrategy &strategy);
			void umul(const BigInt &num1, unsigned int smallNum);

			void umulIp(unsigned int smallNum);

			void udiv(const BigInt &num1, unsigned int smallNum);
			void udivIp(unsigned int smallNum);
	};
}

#endif /* SRC_BIGNUM_BIGINT_HPP_ */
