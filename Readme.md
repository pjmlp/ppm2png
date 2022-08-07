# Introduction

Basically this is a converter for the PPM file format as used on the [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html#outputanimage/theppmimageformat) tutorial.

There are several versions of it, the originally coded in C++20, and the later port done to C#.

Afterwards as I started discussing the performace of Native AOT in .NET 7 preview, I needed to provide some numbers for more arguments, hence how the Go, D, and Java versions came to this repo as well.

# Building it

In C# and C++ cases I only focused on Visual Studio support, although the C# version is cross plaform, whereas the C++ focus on Windows APIs.

Everything else uses their standard build workflows.

# Trying it out

If you don't happen to have PPM file with the P3 format lying around, there is one on the samples folder.
