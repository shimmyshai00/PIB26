/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      Punned.hpp
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

#ifndef SRC_MEMORY_BUFFERS_LOCAL_PUNNED_HPP_
#define SRC_MEMORY_BUFFERS_LOCAL_PUNNED_HPP_

#include "../../ILocalBuffer.hpp"

#include <cstddef>

namespace SDF::Memory::Buffers::Local
{
	// Class:      Punned
	// Purpose:    Implements a pun on a buffer of a different type. Does not own the data.
	// Parameters: U - The punned type of elements held.
	template<class U>
	class Punned : public ILocalBuffer<U>
	{
		public:
			Punned();

			// Function:   Punned
			// Purpose:    Construct a new pun. For internal use only!
			// Parameters: ptr - The pointer to the untyped buffer data
			//             size - The size of the untyped data.
			Punned(void *ptr, std::size_t size);

			std::size_t getSize() const;

			SafePtr<U> accessData(std::size_t idx);
			SafePtr<const U> accessData(std::size_t idx) const;
		private:
			std::size_t m_size;
			U *m_data;
	};
}

#include "Punned.tpp"

#endif /* SRC_MEMORY_BUFFERS_LOCAL_PUNNED_HPP_ */
