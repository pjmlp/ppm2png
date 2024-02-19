# Introduction


Modern C++ version of the conversion utility as a command line application.

# Building

It was developed with Visual Studio 2022 and makes use of C++ 20 modules, and C++23 standard library as module, this setup works perfectly alright with MSBuild.

Maybe when CMake starts supporting modules I can eventually add a build configuration for it.


# Remarks

Maybe it still has bugs, or could be made better, but it gets the job done.

There are some issues out of the static analysis from VS 2022, however they are a mix of false positives and warnings not relevant for this basic use case.

In any case, if security is a concern, there is the C# version, which does the job just as well.