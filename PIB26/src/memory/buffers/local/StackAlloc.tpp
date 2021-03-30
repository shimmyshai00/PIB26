/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      StackAlloc.tpp
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

#ifndef SRC_BIGNUM_BUFFERS_STACKALLOC_TPP_
#define SRC_BIGNUM_BUFFERS_STACKALLOC_TPP_

#include "../../SafePtr.hpp"

#include <cstddef>
#include <cassert>

namespace SDF::Memory::Buffers::Local
{
	template<class T, std::size_t bufSize>
	StackAlloc<T, bufSize>::StackAlloc()
	{
	}

	template<class T, std::size_t bufSize>
	StackAlloc<T, bufSize>::~StackAlloc()
	{
	}

	template<class T, std::size_t bufSize>
	std::size_t StackAlloc<T, bufSize>::getSize() const
	{
		return bufSize;
	}

	template<class T, std::size_t bufSize>
	SafePtr<T> StackAlloc<T, bufSize>::accessData(std::size_t idx) {
#ifndef NDEBUG
		return SafePtr<T>(m_data, m_data + bufSize, m_data + idx);
#else
		return SafePtr<T>(m_data + idx);
#endif // !NDEBUG
	}

	template<class T, std::size_t bufSize>
	SafePtr<const T> StackAlloc<T, bufSize>::accessData(std::size_t idx) const {
#ifndef NDEBUG
		return SafePtr<const T>(m_data, m_data + bufSize, m_data + idx);
#else
		return SafePtr<const T>(m_data + idx);
#endif // !NDEBUG
	}
}

#endif /* SRC_BIGNUM_BUFFERS_STACKALLOC_TPP_ */
