/*
 * PIB26 version 0.0.1
 * (C) 2020-2021 Shimrra Shai.
 * 
 * File:      pib26.cpp
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

#include "bignum/multiplication/ClassicalSmallMul.hpp"
#include "bignum/multiplication/SmallKaratsuba.hpp"
#include "bignum/multiplication/FFT.hpp"
#include "bignum/multiplication/FlexMul3.hpp"

#include "bignum/BigFloat.hpp"
#include "bignum/BigInt.hpp"

#include "pi/bsp/bsp.hpp"
#include "pi/bsp/chudnovsky.hpp"

#include "util/timer.hpp"
#include "util/userInput.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <time.h>

// Function:  main
// Purpose:   The main program.
// Arguments: argc, argv - standard parameters
// Returns:   0 - success
//            1 - error
int main(int argc, char **argv)
{
	using namespace SDF;

	try {
		std::cout << "PIB26 version 0.0.1" << std::endl;
		std::cout << std::endl;

		std::size_t numDigits = Util::getUserNumericInput("Enter number of digits to compute", 100,
			4000000);
		std::cout << std::endl;

		/*
		 bool wantParallel = Util::getUserYNInput("Do you want to use parallel processing");
		 int numThreads;
		 if(wantParallel) {
		 numThreads = Util::getUserNumericInput("Enter number of threads to use", 1, 4);
		 }
		 std::cout << std::endl;
		 */

		struct timespec startTime, endTime;

		clock_gettime(CLOCK_REALTIME, &startTime);

		std::cout << "Allocating memory..." << std::endl;

		Bignum::Multiplication::ClassicalSmallMul smallStrategy(1024);
		Bignum::Multiplication::SmallKaratsuba medStrategy(16384);
		Bignum::Multiplication::FFT largeStrategy(
			std::max<std::size_t>(16384, 2 * numDigits / Bignum::DIGS_PER_DIG) + 16);
		Bignum::Multiplication::FlexMul3 flexStrategy(&smallStrategy, &medStrategy, &largeStrategy,
			128, 2048);
		Pi::BSP::Chudnovsky chudnovsky(&flexStrategy);

		std::cout << "Done." << std::endl;
		std::cout << std::endl;

		std::shared_ptr<Bignum::BigFloat> pi(chudnovsky.computePi(numDigits));

		clock_gettime(CLOCK_REALTIME, &endTime);

		std::cout << "Writing result..." << std::endl;
		pi->printNiceToFile("pi.txt", numDigits);
		std::cout << "Done." << std::endl;
		std::cout << std::endl;
		std::cout << "Total computation time: " << Util::timeDiffMillis(endTime, startTime) << " ms."
			<< std::endl;

		return 0;
	} catch (std::exception &e) {
		std::cout << "ERROR: " << e.what() << ". This error was not handled anywhere else. Aborting."
			<< std::endl;

		return 1;
	}
}
