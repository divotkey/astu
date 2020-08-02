/*
 * ASTU - AST Utilities
 * A collection of Utilities for Advanced Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>

#include <fstream>
#include "Color.h"
#include "Image.h"
#include "BmpCodec.h"

#define BI_RGB			0
#define BI_RLE8			1
#define BI_RLE4			2
#define BI_BITFIELDS	3

#define BYTES_PER_PIXEL 3

#pragma pack(push, 1)

// Note: Bitmap file format uses Little-endian. This code does assume that
// the machine executing this code is using Little-endian as well.

// Bitmap File Header Version 3
struct BitmapFileHeader {
	uint16_t bfType;			// signature BM (0x42 0x4D)
	uint32_t bfSize;			// size of entire file (unreliable)
	uint32_t bfReserved;		// defaults to 0
	uint32_t bfOffBits;			// file offset to pixel array
};

// Bitmap File Header Version 3
struct BitmapInfoHeader {
	uint32_t biSize;			// number of bytes required by the structure
	int32_t biWidth;			// width of the bitmap, in pixels
	int32_t biHeight;			// height of the bitmap, in pixels
	uint16_t biPlanes;			// number of planes for the target device, must be 1
	uint16_t biBitCount;		// number of bits-per-pixel, must be 1, 4, 8, or 24
	uint32_t biCompression;		// type of compression for a compressed bottom - up bitmap
	uint32_t biSizeImage;		// size, in bytes, of the image, may be set to zero for BI_RGB
	int32_t	biXPelsPerMeter;	// horizontal resolution, in pixels-per-meter
	int32_t	biYPelsPerMeter;	// vertical resolution, in pixels-per-meter
	uint32_t biClrUsed;			// number of color indexes in the color table that are actually used
	uint32_t biClrImportant;	// number of color indexes that are required for displaying the bitmap
};

#pragma pack(pop)


namespace astu {

	size_t RoundUp2(size_t v, size_t m)
	{
		assert(m && ((m & (m - 1)) == 0));
		return (v + m - 1) & ~(m - 1);
    }    

    unsigned int CalcNumPadding(unsigned int width, unsigned int bytes)
    {
		return static_cast<unsigned int>(RoundUp2(width * bytes, 4) - width * bytes);        
    }

    BmpEncoder::BmpEncoder()
        : flipVertically(true)
    {
        // Intentionally left empty
    }

    bool BmpEncoder::IsFlipVertically() const
    {
        return flipVertically;
    }

    BmpEncoder & BmpEncoder::SetFlipVertically(bool flip)
    {
        flipVertically = flip;
        return *this;
    }

    void BmpEncoder::Encode(const Image & image, std::ostream& os) const
    {
		// Each line must contain number of bytes dividable by four.
		int numPadding = CalcNumPadding(image.GetWidth(), BYTES_PER_PIXEL);

		// The size of the image in bytes (including ling padding)
		unsigned int sizeOfImage = image.GetHeight() * (image.GetWidth() * BYTES_PER_PIXEL + numPadding);

		// Initialize file header.
		BitmapFileHeader fh;
		fh.bfType = 0x4d42;
		fh.bfSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + sizeOfImage;
		fh.bfReserved = 0;
		fh.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

		// Initialize info header.
		BitmapInfoHeader ih;
		ih.biSize = sizeof(BitmapInfoHeader);
		ih.biWidth = image.GetWidth();
		ih.biHeight = image.GetHeight();
		ih.biPlanes = 1;
		ih.biBitCount = BYTES_PER_PIXEL * 8;
		ih.biCompression = BI_RGB;
		ih.biSizeImage = sizeOfImage;
		ih.biXPelsPerMeter = 0;
		ih.biYPelsPerMeter = 0;
		ih.biClrUsed = 0;
		ih.biClrImportant = 0;
        
		// Write header information
        os.write(reinterpret_cast<const char*>(&fh), sizeof(BitmapFileHeader));
        os.write(reinterpret_cast<const char*>(&ih), sizeof(BitmapInfoHeader));

		// Write image data.
        if (flipVertically) {
			for (int j = image.GetHeight(); j > 0; --j) {
				for (int i = 0; i < image.GetWidth(); ++i) {
					// Write pixel data.
					int pixel = image.GetPixel(i, j - 1).GetARGB();
                    os.write(reinterpret_cast<const char*>(&pixel), 3);
				}

				for (int i = 0; i < numPadding; ++i) {
                    os.put(0);
				}
			}
        } else {
            for (int j = 0; j < image.GetHeight(); ++j) {
                for (int i = 0; i < image.GetWidth(); ++i) {
                    // Write pixel data.
                    int pixel = image.GetPixel(i, j).GetARGB();
                    os.write(reinterpret_cast<const char*>(&pixel), 3);
                }

                for (int i = 0; i < numPadding; ++i) {
                    os.put(0);
                }
            }
        }
    }

    void BmpEncoder::Encode(const Image & image, const char * filename) const
    {
		// Create file.
        std::ofstream ofs(filename, std::ios::out | std::ios::binary);
        if (!ofs) {
            throw std::runtime_error(std::string("unable to open BMP file for writing '") 
				+ filename + "'");
        }

        Encode(image, ofs);
        ofs.close();
    }
}