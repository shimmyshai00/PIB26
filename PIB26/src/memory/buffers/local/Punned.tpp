/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      Punned.tpp
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

#ifndef SRC_MEMORY_BUFFERS_LOCAL_PUNNED_TPP_
#define SRC_MEMORY_BUFFERS_LOCAL_PUNNED_TPP_

#include "Punned.hpp"

#include <cstddef>
#include <cassert>

namespace SDF::Memory::Buffers::Local
{
	template<class U>
	Punned<U>::Punned()
		: m_size(0), m_data(nullptr)
	{
	}

	template<class U>
	Punned<U>::Punned(void *ptr, std::size_t size)
		: m_size(size / sizeof(U)), m_data(reinterpret_cast<U*>(ptr))
	{
	}

	template<class U>
	std::size_t Punned<U>::getSize() const
	{
		return m_size;
	}

	template<class U>
	SafePtr<U> Punned<U>::accessData(std::size_t idx)
	{
#ifndef NDEBUG
		return SafePtr<U>(m_data, m_data + m_size, m_data + idx);
#else
		return SafePtr<U>(m_data + idx);
#endif // !NDEBUG
	}

	template<class U>
	SafePtr<const U> Punned<U>::accessData(std::size_t idx) const
	{
#ifndef NDEBUG
		return SafePtr<const U>(m_data, m_data + m_size, m_data + idx);
#else
		return SafePtr<const U>(m_data + idx);
#endif // !NDEBUG
	}
}

#endif /* SRC_MEMORY_BUFFERS_LOCAL_PUNNED_TPP_ */
