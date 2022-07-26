# Introduction

Basically this is a converter for the PPM file format as used on the [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html#outputanimage/theppmimageformat) tutorial.

It is a port for a small utility that I originally coded in C++20.

# Building

It was developed with Visual Studio 2022 and makes use of .NET 6 alongside C# 10.

Building on Linux and macOS versions should work out of the box with `dotnet build`.

# Trying it out

If you don't happen to have PPM file with the P3 format lying around, there is one on the samples folder.

# Where is the C++ version now

If you still care about the original C++ version, it now lives on its own branch, named [cpp-version](https://github.com/pjmlp/ppm2png/tree/cpp-version).
