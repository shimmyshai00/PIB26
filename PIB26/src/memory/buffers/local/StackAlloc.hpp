/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      StackAlloc.hpp
 * Timestamp: Mar 12, 2021
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

#ifndef SRC_BIGNUM_BUFFERS_STACKALLOC_HPP_
#define SRC_BIGNUM_BUFFERS_STACKALLOC_HPP_

#include "../../ILocalBuffer.hpp"

#include "../../SafePtr.hpp"

#include <cstddef>

namespace SDF::Memory::Buffers::Local
{
	// Class:      StackAlloc
	// Purpose:    Provides a bignum buffer that is allocated on the stack, at a fixed size. Useful
	//             for small, throwaway buffers.
	// Parameters: T - the type of elements held in the buffer.
	//             bufSize - the size of the buffer (fixed!).
	template<class T, std::size_t bufSize>
	class StackAlloc : public ILocalBuffer<T>
	{
		public:
			StackAlloc();
			~StackAlloc();

			std::size_t getSize() const;

			SafePtr<T> accessData(std::size_t idx);
			SafePtr<const T> accessData(std::size_t idx) const;
		private:
			T m_data[bufSize];
	};
}

#include "StackAlloc.tpp"

#endif /* SRC_BIGNUM_BUFFERS_STACKALLOC_HPP_ */
