/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      IDistributedBuffer.hpp
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

#ifndef SRC_MEMORY_IDISTRIBUTEDBUFFER_HPP_
#define SRC_MEMORY_IDISTRIBUTEDBUFFER_HPP_

#include "BufferSegment.hpp"
#include "ConstBufferSegment.hpp"

#include <cstddef>

namespace SDF::Memory {
	// Class:      IDistributedBuffer
	// Purpose:    Provides an interface for memory buffers distributed across multiple machines.
	//             (TBA).
	// Parameters: T - The type of elements in the buffer.
	template<class T>
	class IDistributedBuffer {
		public:
			virtual ~IDistributedBuffer() = 0;

			// Function:   getSize
			// Purpose:    Gets the total size of the buffer.
			// Parameters: None.
			// Returns:    The buffer size in elements.
			virtual std::size_t getSize() const = 0;

			// Function:   getLocalSize
			// Purpose:    Gets the size of the part of the buffer held on this machine.
			// Parameters: None.
			// Returns:    The size of the local segment in elements.
			virtual std::size_t getLocalSize() const = 0;
	};
}

#endif /* SRC_MEMORY_IDISTRIBUTEDBUFFER_HPP_ */
