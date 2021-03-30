/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      timer.cpp
 * Timestamp: Mar 20, 2021
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

#include "timer.hpp"

#include <cstdio>

namespace SDF::Util {
	unsigned long timeDiffMillis(struct timespec endTime, struct timespec startTime) {
		time_t tv_sec_diff;
		long tv_msec_diff;

		time_t borrow(0);
		if(endTime.tv_nsec < startTime.tv_nsec) {
			tv_msec_diff = (1000000000L + endTime.tv_nsec - startTime.tv_nsec) / 1000000;
			borrow = 1;
		} else {
			tv_msec_diff = (endTime.tv_nsec - startTime.tv_nsec) / 1000000;
		}

		tv_sec_diff = endTime.tv_sec - startTime.tv_sec - borrow;

		return tv_sec_diff * 1000UL + tv_msec_diff;
	}
}
