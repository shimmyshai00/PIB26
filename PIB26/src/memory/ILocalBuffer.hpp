/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      LocalBuffer.hpp
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

#ifndef SRC_MEMORY_LOCALBUFFER_HPP_
#define SRC_MEMORY_LOCALBUFFER_HPP_

#include <cstddef>

namespace SDF::Memory {
	template<class T>
	class SafePtr;

	// Class:      ILocalBuffer
	// Purpose:    Provides an interface for memory buffers local to a single machine.
	// Parameters: T - The type of elements in the buffer.
	template<class T>
	class ILocalBuffer {
		public:
			virtual ~ILocalBuffer() = default;

			// Function:   getSize
			// Purpose:    Gets the total size of the buffer.
			// Parameters: None.
			// Returns:    The buffer size in elements.
			virtual std::size_t getSize() const = 0;

			// Function:   accessData
			// Purpose:    Get a pointer to the data in the buffer.
			// Parameters: idx - The position to get a pointer to.
			// Returns:    The pointer to the data at the given position.
			virtual SafePtr<T> accessData(std::size_t idx) = 0;
			virtual SafePtr<const T> accessData(std::size_t idx) const = 0;
	};
}

#endif /* SRC_MEMORY_LOCALBUFFER_HPP_ */
