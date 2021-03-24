/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      RAMOnly.tpp
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

#ifndef SRC_BIGNUM_BUFFERS_RAMONLY_TPP_
#define SRC_BIGNUM_BUFFERS_RAMONLY_TPP_

#include "../../SafePtr.hpp"

#include <cstddef>
#include <cassert>

namespace SDF::Memory::Buffers::Local
{
	template<class T>
	RAMOnly<T>::RAMOnly(std::size_t size)
		: m_size(size), m_data(new T[size])
	{
		for (std::size_t i(0); i < size; ++i) {
			m_data[i] = 0;
		}
	}

	template<class T>
	RAMOnly<T>::~RAMOnly()
	{
		m_size = 0;
		delete[] m_data;
	}

	template<class T>
	std::size_t RAMOnly<T>::getSize() const
	{
		return m_size;
	}

	template<class T>
	SafePtr<T> RAMOnly<T>::accessData(std::size_t idx) {
#ifndef NDEBUG
		return SafePtr<T>(m_data, m_data + m_size, m_data + idx);
#else
		return SafePtr<T>(m_data + idx);
#endif // !NDEBUG
	}

	template<class T>
	SafePtr<const T> RAMOnly<T>::accessData(std::size_t idx) const {
#ifndef NDEBUG
		return SafePtr<const T>(m_data, m_data + m_size, m_data + idx);
#else
		return SafePtr<const T>(m_data + idx);
#endif // !NDEBUG
	}
}

#endif /* SRC_BIGNUM_BUFFERS_RAMONLY_TPP_ */
