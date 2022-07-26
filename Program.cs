/*
 * Program.cs - C# main module for image conversions.
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

using static ppm2png.ImageWriter;

if (args.Length < 2)
{
    string filename = System.Diagnostics.Process.GetCurrentProcess().ProcessName;
    Console.WriteLine($"Missing arguments:\n\n {filename}.exe ppm-filename png-filename\n");
    return 1;
}

string sourceFilename = args[0];
string destFilename = args[1];

// try to get hold of the file.
StreamReader fs;
try
{
    fs = File.OpenText(sourceFilename);
}
catch
{
    Console.Error.WriteLine($"Could not open file {sourceFilename}");
    return 1;
}

// now get into processing it
using (fs)
{
    // validate file header
    const string FileHeader = "P3";
    string? line = fs.ReadLine();
    if (FileHeader != line)
    {
        Console.Error.WriteLine($"Failed to read {sourceFilename}, bad header");
        return 1;
    }

    char[] separators = new char[] { ' ' };

    int width;
    int height;
    line = fs.ReadLine();
    if (line != null)
    {
        string[] values = line.Split(separators, StringSplitOptions.RemoveEmptyEntries);
        width = int.Parse(values[0]);
        height = int.Parse(values[1]);
    }
    else
    {
        Console.Error.WriteLine($"Failed to read {sourceFilename}, bad dimensions");
        return 1;
    }

    int bpp;
    if (!int.TryParse(fs.ReadLine(), out bpp) || bpp <= 0 || bpp >= 256)
    {
        Console.Error.WriteLine($"Failed to read {sourceFilename}, bad colour size");
        return 1;
    }

    // prepare the data buffer
    const int BPP = 3;
    int imageSize = width * height * BPP;

    var imageData = new byte[imageSize];

    int row = 0;
    while ((line = fs.ReadLine()) != null)
    {
        string[] values = line.Split(separators, StringSplitOptions.RemoveEmptyEntries);
        if (values.Length != BPP)
        {
            Console.Error.WriteLine($"Failed to read {sourceFilename} at line {row}, bad colour size");
            return 1;
        }

        imageData[row] = byte.Parse(values[0]);
        imageData[row + 1] = byte.Parse(values[1]);
        imageData[row + 2] = byte.Parse(values[2]);

        // advance for the next pixel group
        row += BPP;
    }

    SaveImage(destFilename, imageData, width, height);

    Console.WriteLine($"Processed {sourceFilename} into {destFilename}");
    return 0;
}