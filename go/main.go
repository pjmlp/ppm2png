/*
 * main.go - Go main module for image conversions.
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

package main
 
import (
    "bufio"
    "fmt"
    "os"
	"strings"
	"strconv"
	"image"
	"image/png"
)

// Utility parse function to convert image values
func parseByte(strValue string) byte {
	var value uint64
	var err error

	if value, err = strconv.ParseUint(strValue, 10, 8); err != nil {
		fmt.Printf("Failed to convert %s: %s\n", strValue, err.Error())
		os.Exit(1)
	}
	return byte(value) 
}

func main() {
	if (len(os.Args) < 2) {
		fmt.Printf("Missing arguments:\n\n %s ppm-filename png-filename\n", os.Args[0])
		os.Exit(1)
	}

	sourceFilename := os.Args[1]
	destFilename := os.Args[2]
 
	// try to get hold of the file.
    readFile, err := os.Open(sourceFilename)
    if err != nil {
        fmt.Printf("Could not open file %s: %s\n", sourceFilename, err.Error())
		os.Exit(1)
    }
	defer readFile.Close()

	// now get into processing it
    fileScanner := bufio.NewScanner(readFile)
 
    fileScanner.Split(bufio.ScanLines)

	// validate file header
    const FileHeader = "P3"
	if fileScanner.Scan() && fileScanner.Text() != FileHeader{
		fmt.Printf("Failed to read %s, bad header\n", sourceFilename)
		os.Exit(1)
	}

	var values []string
	var width, height int
	if fileScanner.Scan() {
		line := fileScanner.Text()

		var value int64
		values = strings.Split(line, " ")
		if len(values) != 2 {
			fmt.Printf("Failed to read %s bad dimensions ->%d\n", sourceFilename, len(values))
			os.Exit(1)
		}

		if value, err = strconv.ParseInt(values[0], 10, 32); err != nil {
			fmt.Printf("Failed to read %s, bad dimensions: %s\n", sourceFilename, err.Error())
			os.Exit(1)
		}
		width = int(value) 

		if value, err = strconv.ParseInt(values[0], 10, 32); err != nil {
			fmt.Printf("Failed to read %s, bad dimensions: %s\n", sourceFilename, err.Error())
			os.Exit(1)
		}
		height = int(value)

	}

	// validate the bpp values, even if don't use them.
	if fileScanner.Scan() {
		line := fileScanner.Text()

		if _, err := strconv.ParseInt(line, 10, 32); err != nil {
			fmt.Printf("Failed to read %s, bad colour size: %s\n", sourceFilename, err.Error())
			os.Exit(1)
		}
	}	

	// prepare the data buffer
	const BPP = 4

	imageData := image.NewRGBA(image.Rect(0, 0, width, height))
  
	row := 0

    for fileScanner.Scan() {
		line := fileScanner.Text()
		
		values := strings.Split(line, " ")
		if len(values) != 3 {
			fmt.Println(values)
			fmt.Printf("Failed to read %s at line %d, bad colour size\n", sourceFilename, row)
			os.Exit(1)
		}

		imageData.Pix[row] = parseByte(values[0])
        imageData.Pix[row + 1] = parseByte(values[1])
        imageData.Pix[row + 2] = parseByte(values[2])
		imageData.Pix[row + 3] = 255

        // advance for the next pixel group
        row += BPP
    }

	outputFile, err := os.Create(destFilename)
    if err != nil {
        fmt.Printf("Could not create file %s: %s\n", destFilename, err.Error())
		os.Exit(1)
    }
	defer outputFile.Close()	

	png.Encode(outputFile, imageData)

    fmt.Printf("Processed %s into %s\n", sourceFilename, destFilename)
	os.Exit(0)
}