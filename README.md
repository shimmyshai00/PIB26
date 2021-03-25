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

The current program is in an extremely immature stage - at version 0.0.1 it is still considered mostly a naive program, and doesn't support the base-26 calculation yet - only supporting decimal like other programs. It is very slow by comparison and nowhere near the league of the programs mentioned, taking a full 16.7 seconds to compute 1 million digits (1,000,000) of decimal pi on a decent processor - Core i3-8100, on a single core. But - right now, it works - and thus provides a ready base for continued improvement and optimization. We also invite feedback and contributions as to how it can be made better.

NEW (UE+1616.63 Ms [2021-03-24]): The program now can compute 1M in under 9s.

Building
========
The program code is an Eclipse project and .mk makefiles, however you do not need Eclipse itself to compile the program (but it is definitely recommended if you want to contribute changes). The program can be compiled on any UNIX-style system with suitable C++ compiler and standard toolchain (e.g. g++ and make) by changing to the "Debug" or "Release" version directory and then running "make". The Debug build is much slower than the Release build, since compiler optimization is not turned on. The timings mentioned are given for the Release version, which is considered the "proper" one to benchmark.
