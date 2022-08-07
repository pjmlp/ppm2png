/*
 * main.d - D main module for image conversions.
 * Copyright (C) 2022  Paulo Pinto
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

import std.stdio;
import std.string;
import std.conv;
import imageformats;


int main(string[] args) {
    if (args.length < 3) {
        writef("Missing arguments:\n\n %s ppm-filename png-filename\n", args[0]);
        return 1;
    }

    string sourceFilename = args[1];
    string destFilename = args[2];    

    // try to get hold of the file.
    File fs;
    try {
        fs = File(sourceFilename, "r");
    }
    catch(Exception ex) {
        writefln("Could not open file %s", sourceFilename);
        return 1;
    }

    // validate file header
    const string FileHeader = "P3";
    string line = strip(fs.readln());
    if (line != FileHeader) {
        writefln("Failed to read %s, bad header", sourceFilename);
        return 1;
    }

    int width;
    int height;
    line = fs.readln();
    if (line !is null) {
        auto values = line.strip().split();
        if (values.length != 2) {
            writefln("Failed to read %s, bad dimensions", sourceFilename);
            return 1;
        }

        width = to!int(values[0]);
        height = to!int(values[1]);
    }

    int bpp;
    line = fs.readln();
    if (line !is null) {
        bpp = to!int(line.strip());
        if (bpp <= 0 || bpp >= 256) {
            writefln("Failed to read %s, bad colour size", sourceFilename);
            return 1;
        }
    }

    // prepare the data buffer
    const int BPP = 3;
    int imageSize = width * height * BPP;

    auto imageData = new ubyte[imageSize];

    int row = 0;
    while ((line = fs.readln()) !is null) {
        string[] values = line.strip().split();
        if (values.length != BPP) {
            writefln("Failed to read %s at line %d, bad colour size", sourceFilename, row);
            return 1;
        }

        imageData[row] = to!ubyte(values[0]);
        imageData[row + 1] = to!ubyte(values[1]);
        imageData[row + 2] = to!ubyte(values[2]);

        // advance for the next pixel group
        row += BPP;
    }

    write_png(destFilename, width, height, imageData);

    writefln("Processed %s into %s",sourceFilename, destFilename);
    return 0;
}