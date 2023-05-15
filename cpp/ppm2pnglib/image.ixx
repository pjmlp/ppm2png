/*
 * image.ixx - C++ module for image conversions.
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
module;

// ensures safety checks are turned on even on release mode
#define _ITERATOR_DEBUG_LEVEL 1

#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <span>
#include <optional>

export module image;

import gdiplusinitialize;

using namespace Gdiplus;

/// <summary>
/// Kinds of available encodings to save the image to.
/// </summary>
export enum class ImageEncondings { PNG };

// forward declarations for private stuff;
bool SaveImage(const std::wstring& filename, ImageEncondings enconding, int imageWidth, int imageHeight, BYTE* pImageData);

/// <summary>
/// Creates the image file. It assumes the image data is stored bottom up.
/// </summary>
/// <param name="filename">The filename as wide string format.</param>
/// <param name="enconding">The desired image format</param>
/// <param name="imageWidth">The image data width</param>
/// <param name="imageHeight">The image data heigh</param>
/// <param name="imageData">Image data buffer.</param>
export void CreateImage(const std::wstring& filename, ImageEncondings enconding, int imageWidth, int imageHeight, BYTE* imageData)
{
   gdiplusinitialize gdiPlus;

   SaveImage(filename, enconding, imageWidth, imageHeight, imageData);
}

/// <summary>
/// Creates the image file. It assumes the image data is stored bottom up.
/// </summary>
/// <param name="filename">The filename.</param>
/// <param name="enconding">The desired image format</param>
/// <param name="imageWidth">The image data width</param>
/// <param name="imageHeight">The image data heigh</param>
/// <param name="imageData">Image data buffer.</param>
export void CreateImage(const std::string& filename, ImageEncondings enconding, int imageWidth, int imageHeight, BYTE* imageData)
{
    gdiplusinitialize gdiPlus;

    // Windows APIs expect to deal with wide strings anyway
    std::wstring wfilename(filename.begin(), filename.end());

    SaveImage(wfilename, enconding, imageWidth, imageHeight, imageData);
}

/// <summary>
/// Reads the PPM file.
/// </summary>
/// <param name="filename">The filename to be read.</param>
/// <returns>The image data is successful</returns>
export std::vector<BYTE> ReadPPMImage(const std::string_view filename, int& width, int& height)
{
    std::ifstream fd(filename.data());
    if (!fd) {
        std::cerr << "Failed to read " << filename << std::endl;
        return {};
    }

    // validate file header
    constexpr auto FileHeader = "P3";
    std::string line;
    fd >> line;
    if (line != FileHeader) {
        std::cerr << "Failed to read " << filename << ", bad header!\n";
        return {};
    }

    //int width = 0;
    //int height = 0;
    fd >> width >> height;
    if (width <= 0 || height <= 0) {
        std::cerr << "Failed to read " << filename << ", bad dimensions!\n";
        return {};
    }

    int bpp = 0;
    fd >> bpp;
    if (bpp <= 0 || bpp >= 256) {
        std::cerr << "Failed to read " << filename << ", bad color size!\n";
        return {};
    }

    // prepare the data buffer
    constexpr auto BPP = 3;
    const std::size_t imageSize = static_cast<size_t>(width) * height * BPP;

    std::vector<BYTE> imageData(imageSize);

    std::size_t row = 0;
    std::string r, g, b;
    for (row = 0; row < imageSize && fd; row += BPP) {
        fd >> r >> g >> b;

        // Windows bitmaps are BGR
        imageData[row] = static_cast<BYTE> (std::stoi(b));
        imageData[row + 1] = static_cast<BYTE> (std::stoi(g));
        imageData[row + 2] = static_cast<BYTE> (std::stoi(r));
    }

    return imageData;
}

module :private;

/// <summary>
/// standard convertion to_wstring() overload for the given enumeration type.
/// </summary>
/// <param name="value">The value to convert into a string.</param>
/// <returns>String representation as wide string value</returns>
std::wstring to_wstring(ImageEncondings value) {
    return L"image/png";
}

/// <summary>
/// Retrieves the desired enconder, if any.
/// </summary>
/// <param name="format">MIME type for the format, e.g "image/png"</param>
/// <returns>The CLSID GUID if found, none otherwise</returns>
std::optional<CLSID> GetEncoderClsid(std::wstring_view format)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return {};

    std::unique_ptr<ImageCodecInfo[]> imageCodecInfo;
    try {
        imageCodecInfo = std::make_unique<ImageCodecInfo[]>(size);
    }
    catch (std::bad_alloc err) {
        return {};  // Failure
    }

    GetImageEncoders(num, size, imageCodecInfo.get());

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(imageCodecInfo[j].MimeType, format.data()) == 0)
        {
            auto clsid = imageCodecInfo[j].Clsid;
            return clsid;  // Success
        }
    }

    return {};  // Failure
}

/// <summary>
/// Saves the actual image data.
/// </summary>
/// <param name="filename">The filename in Windows format</param>
/// <param name="enconding">What kind of image format to use</param>
/// <param name="imageWidth">The image width.</param>
/// <param name="imageHeight">The image width.</param>
/// <param name="pImageData">Image buffer</param>
/// <return>True if the image could be saved</return>
bool SaveImage(const std::wstring& filename, ImageEncondings enconding, int imageWidth, int imageHeight, BYTE* pImageData)
{
    //Create the bitmap header.
    BITMAPINFO bmi = {};											
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = imageWidth;
    bmi.bmiHeader.biHeight = -imageHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount = 24;

    // now convert the image
    Bitmap bmp(&bmi, pImageData);

    auto clsid = GetEncoderClsid(to_wstring(enconding));
    if (clsid.has_value()) {
        bmp.Save(filename.c_str(), &clsid.value());
        return true;
    }

    return false;
}