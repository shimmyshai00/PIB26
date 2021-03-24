/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      RAMOnly.hpp
 * Timestamp: Mar 3, 2021
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

#ifndef SRC_BIGNUM_BUFFERS_RAMONLY_HPP_
#define SRC_BIGNUM_BUFFERS_RAMONLY_HPP_

#include "../../ILocalBuffer.hpp"
#include "Punned.hpp"

#include <cstddef>

namespace SDF::Memory::Buffers::Local
{
	// Class:      RAMOnly
	// Purpose:    Provides a RAM-only (heap allocated) local buffer.
	// Parameters: T - the type of elements held in the buffer.
	template<class T>
	class RAMOnly : public ILocalBuffer<T>
	{
		public:
			// Function:  RAMOnly
			// Purpose:   Construct a new buffer with a given size.
			// Arguments: size - The size of the new buffer.
			RAMOnly(std::size_t size);
			~RAMOnly();

			std::size_t getSize() const;

			SafePtr<T> accessData(std::size_t idx);
			SafePtr<const T> accessData(std::size_t idx) const;

			// Function:   pun
			// Purpose:    "Puns" the buffer to allow a different data type to be stored in it. USE
			//             WITH CAUTION!
			// Parameters: None.
			// Returns:    The punned buffer.
			template<class U>
			Punned<U> pun() {
				return Punned<U>(m_data, m_size * sizeof(T));
			}
		private:
			std::size_t m_size;
			T *m_data;
	};
}

#include "RAMOnly.tpp"

#endif /* SRC_BIGNUM_BUFFERS_RAMONLY_HPP_ */
