# Introduction

Basically this is a converter for the PPM file format as used on the [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html#outputanimage/theppmimageformat) tutorial.

It was coded in one afternoon, while I was playing around with modern C++ features.

# Building

It was developed with Visual Studio 2022 and makes use of modules, this setup works perfectly alright with MSBuild.

Maybe when CMake starts supporting modules I can eventually add a build configuration for it.


# Remarks

Maybe it has bugs, or could be made better in so many ways (including how the PPM gets parsed).

Some effort was spent running it through the static analysis tooling form VS 2022, and no major issues were found, however they are known not to work 100% with modules still.

In what concerns porting my hobby renderings output from PPM into PNG, when I am on a Windows development environment, it is more than good enough.