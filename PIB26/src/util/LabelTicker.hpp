/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      LabelTicker.hpp
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

#ifndef SRC_UTIL_LABELTICKER_HPP_
#define SRC_UTIL_LABELTICKER_HPP_

#include "ITicker.hpp"

#include <string>

namespace SDF::Util {
	// Class:   LabelTicker
	// Purpose: Displays a ticker with a label and a numerical progress indicator in the form "...
	//          / ...", where the "/" stands for "of".
	class LabelTicker : public ITicker {
		public:
			LabelTicker(std::string label);

			std::size_t getTickerMax() const;
			std::size_t getTickerCur() const;

			void setTickerMax(std::size_t max);
			void setTickerCur(std::size_t cur);

			void printTicker();
			void finishTicker();
		private:
			std::string m_label;

			std::size_t m_tickerMax;
			std::size_t m_tickerCur;
	};
}

#endif /* SRC_UTIL_LABELTICKER_HPP_ */
