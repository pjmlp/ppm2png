/*
 * PPM2PNG.java - Java main module for image conversions.
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

package org.progtools.ppm2png;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.File;

import java.awt.image.BufferedImage;

import javax.imageio.ImageIO;

public class PPM2PNG {
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Missing arguments:\n\n app-name ppm-filename png-filename\n");
            System.exit(1);

        }

        final String sourceFilename = args[0];
        final String destFilename = args[1];

        // try to get hold of the file.
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader(sourceFilename));
        } catch (IOException ex) {
            System.err.printf("Could not open file %s \n", sourceFilename);
            System.exit(1);
        }

        // now get into processing it
        try (var imageReader = reader) {
            // validate file header
            final String FileHeader = "P3";
            String line = imageReader.readLine();
            if (!FileHeader.equals(line)) {
                System.err.printf("Failed to read %s, bad header\n", sourceFilename);
                System.exit(1);
            }

            int width = 0;
            int height = 0;

            line = imageReader.readLine();
            if (line != null) {
                 String[] values = line.split(" ");
                 width = Integer.parseInt(values[0]);
                 height = Integer.parseInt(values[0]);
            } else {
                System.err.printf("Failed to read %s, bad dimensions\n", sourceFilename);
                System.exit(1);
            }


            // just validate the bpp value, even though it isn't being used
            line = imageReader.readLine();
            if (line != null) {
                 int bpp = Integer.parseInt(line);
                 if (bpp < 0 || bpp > 255) {
                    System.err.printf("Failed to read %s, bad colour size\n", sourceFilename);
                    System.exit(1);
                 }
            } else {
                System.err.printf("Failed to read %s, bad colour size\n", sourceFilename);
                System.exit(1);
            }   

            // prepare the data buffer
            final int BPP = 3;
            int imageSize = width * height;

            var imageData = new int[imageSize];

            int row = 0;
            while ((line = imageReader.readLine()) != null) {
                String[] values = line.split(" ");
                if (values.length != BPP) {
                    System.err.printf("Failed to read %s at line %d, bad colour size", sourceFilename, row);
                    System.exit(1);
                }

                int r = Integer.parseInt(values[0]);
                int g = Integer.parseInt(values[1]);
                int b = Integer.parseInt(values[2]);

                int pixelValue = (r << 16) | (g << 8) | b;
                imageData[row++] = pixelValue;
            }

             var bufferImg = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
             bufferImg.setRGB(0, 0, width, height, imageData, 0, width);
             
             var outputfile = new File(destFilename);
             ImageIO.write(bufferImg, "png", outputfile);

        } catch (IOException ex) {
            System.err.printf("Failed to process file %s \n", sourceFilename);
            System.exit(1);
        }


        System.out.printf("Processed %s into %s\n", sourceFilename, destFilename);
        System.exit(0);
    }
}