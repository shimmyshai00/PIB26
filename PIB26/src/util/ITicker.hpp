/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      ITicker.hpp
 * Timestamp: Mar 21, 2021
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

#ifndef SRC_UTIL_ITICKER_HPP_
#define SRC_UTIL_ITICKER_HPP_

#include <cstddef>

namespace SDF::Util {
	// Class:   ITicker
	// Purpose: Provides an interface for different kinds of progress/time display tickers.
	class ITicker {
		public:
			virtual ~ITicker() = default;

			// Function:   getTickerMax
			// Purpose:    Gets the maximum value of the ticker counter when full.
			// Parameters: None.
			// Returns:    The value when full.
			virtual std::size_t getTickerMax() const = 0;

			// Function:   getTickerCur
			// Purpose:    Gets the current value of the ticker.
			// Parameters: None.
			// Returns:    The current ticker value.
			virtual std::size_t getTickerCur() const = 0;

			// Function:   setTickerMax
			// Purpose:    Sets the maximum value of the ticker counter when full.
			// Parameters: max - The new maximum value.
			// Returns:    None.
			virtual void setTickerMax(std::size_t max) = 0;

			// Function:   setTickerCur
			// Purpose:    Sets the currently-displayed value of the ticker.
			// Parameters: cur - The new ticker value.
			// Returns:    None.
			virtual void setTickerCur(std::size_t cur) = 0;

			// Function:   printTicker
			// Purpose:    Updates the printed ticker. Note: because this is crude, we cannot guarantee
			//             the ticker will still look correct if something else is output to the
			//             console before this is called!
			// Parameters: None.
			// Returns:    None.
			virtual void printTicker() = 0;

			// Function:   finishTicker
			// Purpose:    Finishes the ticker session, making it okay to print new stuff to the
			//             screen.
			// Parameters: None.
			// Returns:    None.
			virtual void finishTicker() = 0;
	};
}

#endif /* SRC_UTIL_ITICKER_HPP_ */
