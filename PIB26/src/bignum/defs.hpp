/*
 * PIB26 version 0.0.2
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      defs.hpp
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

#ifndef SRC_BIGNUM_DEFS_HPP_
#define SRC_BIGNUM_DEFS_HPP_

namespace SDF::Bignum {
	// Constant and type definitions used by the bignum package.
	static const unsigned int BASE = 456976;      // The internal bignum base.
	static const unsigned int BASE_MINOR = 26;    // The small base dividing BASE.
	static const unsigned int DIGS_PER_DIG = 4;   // BASE should equal BASE_MINOR^DIGS_PER_DIG.
	static const unsigned int DIGS_PER_SMALL = 2; // The minimum number of base-BASE digits required
	                                              // to hold a small number (unsigned int).

	typedef int Digit;     // Should be able to hold a number up to BASE-1.
	typedef long TwoDigit; // SHould be able to hold a number up to (BASE^2)-1.

	// Sign enumeration.
	enum Sign {
		SIGN_POSITIVE = +1,
		SIGN_NEGATIVE = -1
	};
}

#endif /* SRC_BIGNUM_DEFS_HPP_ */
