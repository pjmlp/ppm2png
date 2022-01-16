/*
 * main.ixx - C++ main module for image conversions.
 * Copyright (C) 2021  Paulo Pinto
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 // ensures safety checks are turned on even on release mode
#define _ITERATOR_DEBUG_LEVEL 1

#include <windows.h>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <span>

import image;

int main(int argc, char *argv[])
{
    using namespace std::string_literals;

    if (argc < 2) {
        // just provide the bare bones executable name if called with full path
        auto filename = std::filesystem::path(argv[0]).filename();
        std::cerr << "Missing arguments:\n\n" << filename << " ppm-filename png-filename\n";
        return 1;
    }

    // make the arguments more readable and independent of their order
    const std::string sourceFilename = argv[1];
    const std::string destFilename = argv[2];

    std::ifstream fd(sourceFilename);
    if (!fd) {
        std::cerr << "Failed to read " << sourceFilename << std::endl;
        return 1;
    }

    // validate file header
    constexpr auto FileHeader = "P3";
    std::string line;
    fd >> line;
    if (line != FileHeader) {
        std::cerr << "Failed to read " << sourceFilename << ", bad header!\n";
        return 1;
    }

    int width = 0;
    int height = 0;
    fd >> width >> height;
    if (width <= 0 || height <= 0) {
        std::cerr << "Failed to read " << sourceFilename << ", bad dimensions!\n";
        return 1;
    }

    int bpp = 0;
    fd >> bpp;
    if (bpp <= 0 || bpp >= 256) {
        std::cerr << "Failed to read " << sourceFilename << ", bad color size!\n";
        return 1;
    }

    // prepare the data buffer
    constexpr auto BPP = 3;
    std::size_t imageSize = width * height * BPP;

    auto imageData = std::make_unique<BYTE[]>(imageSize);

    std::size_t row = 0;
    std::string r, g, b;
    for (row = 0; row < imageSize && fd; row += BPP) {
        fd >> r >> g >> b;

        // Windows bitmaps are BGR
        imageData[row] = static_cast<BYTE> (std::stoi(b));
        imageData[row + 1] = static_cast<BYTE> (std::stoi(g));
        imageData[row + 2] = static_cast<BYTE> (std::stoi(r));
    }

    CreateImage(destFilename, ImageEncondings::PNG, width, height, imageData.get());

    std::cout << "Processed " << sourceFilename << " into " << destFilename << std::endl;
    return 0;
}