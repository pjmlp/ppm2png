# Introduction

Java version of the conversion utility as a command line application.

# Building with Maven

As with many Java projects based on Maven, a `mvn compile` will be enough.

# Building with GraalVM

Assuming GraaVM is on the path, issue a `native-image org.progtools.ppm2png.PPM2PNG` from the _src/main/java_ directory.

I plan to improve the build instructions and add a Maven profile later on.