/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      DotsTicker.cpp
 * Timestamp: Mar 31, 2021
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

#include "DotsTicker.hpp"
#include <cstdio>

namespace SDF::Util
{
	DotsTicker::DotsTicker(std::string label, std::size_t numDots)
		: m_label(label), m_numDots(numDots), m_tickerMax(100), m_tickerCur(0), m_tickerPrinted(
			false), m_lastTickerPrintVal(0)
	{
	}

	std::size_t DotsTicker::getTickerMax() const
	{
		return m_tickerMax;
	}

	std::size_t DotsTicker::getTickerCur() const
	{
		return m_tickerCur;
	}

	void DotsTicker::setTickerMax(std::size_t max)
	{
		m_tickerMax = max;
	}

	void DotsTicker::setTickerCur(std::size_t cur)
	{
		m_tickerCur = cur;
	}

	void DotsTicker::printTicker()
	{
		std::size_t tickerPrintVal(m_numDots * m_tickerCur / m_tickerMax);
		if (!m_tickerPrinted || (tickerPrintVal != m_lastTickerPrintVal)) {
			printf("%s : [", m_label.c_str());

			for (std::size_t i(0); i < m_numDots; ++i) {
				if (i <= tickerPrintVal) {
					printf(".");
				} else {
					printf(" ");
				}
			}

			printf("]\r");
			fflush(stdout);

			m_tickerPrinted = true;
			m_lastTickerPrintVal = tickerPrintVal;
		}
	}

	void DotsTicker::finishTicker()
	{
		printf("\n");

		m_tickerPrinted = false;
		m_lastTickerPrintVal = 0;
	}
}

