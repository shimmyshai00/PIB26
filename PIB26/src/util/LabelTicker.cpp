/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      LabelTicker.cpp
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

#include "LabelTicker.hpp"
#include <cstdio>

namespace SDF::Util
{
	LabelTicker::LabelTicker(std::string label)
		: m_label(label), m_tickerMax(100), m_tickerCur(0)
	{
	}

	std::size_t LabelTicker::getTickerMax() const
	{
		return m_tickerMax;
	}

	std::size_t LabelTicker::getTickerCur() const
	{
		return m_tickerCur;
	}

	void LabelTicker::setTickerMax(std::size_t max)
	{
		m_tickerMax = max;
	}

	void LabelTicker::setTickerCur(std::size_t cur)
	{
		m_tickerCur = cur;
	}

	void LabelTicker::printTicker()
	{
		printf("%s : %lu / %lu\r", m_label.c_str(), m_tickerCur, m_tickerMax);
		fflush(stdout);
	}

	void LabelTicker::finishTicker()
	{
		printf("\n");
	}
}
