/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      SafePtr.hpp
 * Timestamp: Mar 16, 2021
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

#ifndef SRC_MEMORY_SAFEPTR_HPP_
#define SRC_MEMORY_SAFEPTR_HPP_

#include <cstddef>
#include <cassert>

namespace SDF::Memory
{
	// Class:     SafePtr
	// Purpose:   Provides a pointer into a buffer that is bounds-checked in debug build mode while
	//            becoming effectively a raw pointer in release mode so that it is fast.
	// Arguments: T - The type of data pointed to.
	template<class T>
	class SafePtr
	{
		public:
#ifndef NDEBUG
			SafePtr()
				: m_start(nullptr), m_end(nullptr), m_ptr(nullptr)
			{
			}

			SafePtr(T *start, T *end, T *ptr)
				: m_start(start), m_end(end), m_ptr(ptr)
			{
			}
#else
			SafePtr()
				: m_ptr(nullptr)
			{}

			SafePtr(T *ptr)
				: m_ptr(ptr)
			{}
#endif // !NDEBUG
			~SafePtr()
			{
			}

			// Function:  operator SafePtr<const T>
			// Purpose:   Defines a conversion to a const form of this pointer.
			// Arguments: ptr - The pointer to convert.
			// Returns:   The const version of ptr.
			operator SafePtr<const T>() const
			{
#ifndef NDEBUG
				return SafePtr<const T>(m_start, m_end, m_ptr);
#else
				return SafePtr<const T>(m_ptr);
#endif // !NDEBUG
			}
			// Function:  operator*
			// Purpose:   Dereferences the safe pointer.
			// Arguments: None.
			// Returns:   The value of the dereferenced pointer.
			T& operator*()
			{
				return *m_ptr;
			}

			const T& operator*() const
			{
				return *m_ptr;
			}

			// Function:  operator->
			// Purpose:   Structure access.
			// Arguments: None.
			// Returns:   The structure access pointer.
			T* operator->() const {
				return m_ptr;
			}

			// Function:  operator[]
			// Purpose:   Indexes the safe pointer.
			// Arguments: idx - The index to get.
			// Returns:   The value of the indexed element.
			T& operator[](std::size_t idx)
			{
				assert(m_ptr + idx < m_end);

				return m_ptr[idx];
			}

			const T& operator[](std::size_t idx) const
			{
				assert(m_ptr + idx < m_end);

				return m_ptr[idx];
			}

			// Function:   operator+=, operator-=, operator++, operator--
			// Purpose:    Offsets a safe pointer.
			// Arguments:  offs - The amount to offset by.
			// Returns:    The offset pointer.
			inline SafePtr<T>& operator+=(std::ptrdiff_t offs)
			{
				assert((m_ptr + offs >= m_start) && (m_ptr + offs <= m_end));
				m_ptr += offs;

				return *this;
			}

			inline SafePtr<T>& operator-=(std::ptrdiff_t offs)
			{
				assert((m_ptr - offs >= m_start) && (m_ptr - offs <= m_end));
				m_ptr -= offs;

				return *this;
			}

			inline SafePtr<T>& operator++() {
				assert(m_ptr < m_end);
				++m_ptr;

				return *this;
			}

			inline SafePtr<T>& operator--() {
				assert(m_ptr > m_start);
				--m_ptr;

				return *this;
			}

			// Function:   operaotr==, operator!=, operator<=, operator>=, operator<, operator>
			// Purpose:    Compare the offsets of two safe pointers. They must point into the same
			//             buffers.
			// Arguments:  lhs, rhs - The pointers to compare.
			// Returns:    Whether the given comparison relation is true or not.
			inline friend bool operator==(const SafePtr<T> &lhs, const SafePtr<T> &rhs)
			{
				assert((lhs.m_start == rhs.m_start) && (lhs.m_end == rhs.m_end));

				return lhs.m_ptr == rhs.m_ptr;
			}

			inline friend bool operator!=(const SafePtr<T> &lhs, const SafePtr<T> &rhs)
			{
				assert((lhs.m_start == rhs.m_start) && (lhs.m_end == rhs.m_end));

				return lhs.m_ptr != rhs.m_ptr;
			}

			inline friend bool operator<=(const SafePtr<T> &lhs, const SafePtr<T> &rhs)
			{
				assert((lhs.m_start == rhs.m_start) && (lhs.m_end == rhs.m_end));

				return lhs.m_ptr <= rhs.m_ptr;
			}

			inline friend bool operator<(const SafePtr<T> &lhs, const SafePtr<T> &rhs)
			{
				assert((lhs.m_start == rhs.m_start) && (lhs.m_end == rhs.m_end));

				return (lhs.m_ptr <= rhs.m_ptr) && (lhs.m_ptr != rhs.m_ptr);
			}

			inline friend bool operator>=(const SafePtr<T> &lhs, const SafePtr<T> &rhs)
			{
				assert((lhs.m_start == rhs.m_start) && (lhs.m_end == rhs.m_end));

				return lhs.m_ptr >= rhs.m_ptr;
			}

			inline friend bool operator>(const SafePtr<T> &lhs, const SafePtr<T> &rhs)
			{
				assert((lhs.m_start == rhs.m_start) && (lhs.m_end == rhs.m_end));

				return (lhs.m_ptr >= rhs.m_ptr) && (lhs.m_ptr != rhs.m_ptr);
			}

			// Function:   operator+, operator-
			// Purpose:    Offsets a safe pointer out-of-place.
			// Arguments:  ptr - The pointer to offset.
			//             offs - The amount to offset by.
			// Returns:    The offset pointer.
			inline friend SafePtr<T> operator+(const SafePtr<T> &lhs, std::ptrdiff_t offs)
			{
				return SafePtr<T>(lhs) += offs;
			}

			inline friend SafePtr<T> operator-(const SafePtr<T> &lhs, std::ptrdiff_t offs)
			{
				return SafePtr<T>(lhs) -= offs;
			}

			// Function:  operator-
			// Purpose:   Gets the distance between two pointers. Both must be from the same buffer.
			// Arguments: lhs, rhs - The pointers to difference.
			// Returns:   The distance.
			inline friend std::ptrdiff_t operator-(const SafePtr<T> &lhs, const SafePtr<T> &rhs)
			{
				assert((lhs.m_start == rhs.m_start) && (lhs.m_end == rhs.m_end));

				return lhs.m_ptr - rhs.m_ptr;
			}
		private:
#ifndef NDEBUG
			T *m_start, *m_end; // bounds for bounds checking
#endif // !NDEBUG
			T *m_ptr;
	};
}

#endif /* SRC_MEMORY_SAFEPTR_HPP_ */
