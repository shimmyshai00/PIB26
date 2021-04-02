# PIB26
An open source Pi (3.141592...) calculator, aimed at calculating in base-26 (A...Z) instead of just decimal.

What is PIB26?
==============

PIB26 is a program that calculates huge quantities (millions and billions) of the digits of pi (the mathematical constant, equal to about 3.14). Unlike most such programs, it aims to compute pi in base-26, instead of base-10, or decimal. Base-26 is a way to represent numbers as text, by using 26 digits given by the letters of the English alphabet, instead of only 10 digits, given by the usual numeric symbols. This idea was inspired by, among other things, a very old column this author read around the turn of the 21st century called "The Pi Code" by Mike Keith. Written this way, Pi looks like:

     D.DRSQLOLYRT...
     
instead of

     3.1415926535...

. (Some might find it funny that the first recognizable string in Pi is seen right above as "LOL" ... :D). This author aims to scale up and improve this program to the point where it becomes a first-rate calculator in the same league as popular ones such as Pifast, TachusPI and the legendary Y-Cruncher by Alexander J. Yee - only being fully open source, and ideally with enough commentary and !!!hopefully!!! somewhat readable code so that it may be of use to others seeking to learn how such software works, for the problem of calculating pi to enormous numbers of digits is a good exercise in coding efficiency and a number of cool mathematical algorithms such as the Fast Fourier Transform (FFT). The author of this program seriously lamented the lack of such an open code as a learning resource in his youth years, and wants this program to be the one to provide such for future generations of computer programmers.

Current status
==============

As of version 0.0.2, the current program is still at a relatively immature stage, however it now performs its computations in base-26 as it is was intended to, meaning that, unlike other pi programs out there, your pi will come out as letters instead of numbers. This is what we want! It is, however, not yet ready to call as a properly mature program, for which we've set as a goal having higher speed and supporting multi-threading capability. Note that in comparing this program against other programs, one can/should no longer simply compare the computation times for the number of digits directly, because a base-26 run contains more information per digit: instead, you should compare to a decimal run of a factor of log_10(26) ~ 1.415 times longer. On my Core i3-8100 machine, 706,727 digits (informational content equivalent to 1,000,000 decimal digits) takes on the order of 4600 ms to compute, and the goal is to improve speed by another factor of 4 for the 0.1.0 production release - actually a bit more, to under 1000 ms on my test system. Memory optimizations are another area that needs work, especially now with the introduction of the root tables to improve FFT performance and also to combat rounding error, which has come with somewhat of a memory penalty.

UPDATE (2021-04-01, UE+1617.34 Ms): In the next version (0.0.3), and current repository source, we have brought the 1M time down to 2500 ms thanks to implementing the right-angle FFT optimization. Now we're getting into interesting territory, because we still need a bit more than one more performance doubling to finally reach the desired goal and most of the "simple" optimizations are now done. It's an interesting observation that the standing FFT implementation is now so fast it beats out the Karatsuba method even at quite small input sizes. I suspect some work on that might also help by making the small chunks of the BSP summing faster.

Building
========
The program code is an Eclipse project and .mk makefiles, however you do not need Eclipse itself to compile the program (but it is definitely recommended if you want to contribute changes). The program can be compiled on any UNIX-style system with suitable C++ compiler and standard toolchain (e.g. g++ and make) by changing to the "Debug" or "Release" version directory and then running "make". The Debug build is much slower than the Release build, since compiler optimization is not turned on. The timings mentioned are given for the Release version, which is considered the "proper" one to benchmark.
