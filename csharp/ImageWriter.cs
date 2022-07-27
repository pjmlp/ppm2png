/*
 * ImageWriter.cs - C# Utility class to save the image.
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

using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;


namespace ppm2png;

/// <summary>
/// Utility class to save the renderings as proper images.
/// </summary>
public static class ImageWriter
{
    /// <summary>
    /// Saves the given buffer using the filename extension as format.
    /// </summary>
    /// <param name="filename">The full pathname for the filename being written</param>
    /// <param name="buffer">The image data stored as RGB buffer.</param>
    /// <param name="width">The image width in pixels.</param>
    /// <param name="height">The image height in pixels.</param>
    public static void SaveImage(string filename, ReadOnlySpan<byte> buffer, int width, int height)
    {
        using var image = Image.LoadPixelData<Rgb24>(buffer, width, height);
        image.Save(filename);
    }
}