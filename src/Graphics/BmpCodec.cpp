/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/BmpCodec.h"
#include "Graphics/Color.h"
#include "Graphics/Image.h"
#include "Util/StringUtils.h"

// C++ Standard Library includes
#include <cassert>
#include <fstream>
#include <string>
#include <stdexcept>

#define BI_RGB			0
#define BI_RLE8			1
#define BI_RLE4			2
#define BI_BITFIELDS	3

#define BI_INFOHEADER_A 0
#define BI_INFOHEADER_B 1
#define BI_INFOHEADER_C 2

#define BYTES_PER_PIXEL 3

#pragma pack(push, 1)
//#pragma pack(1)

// TODO: use stream buffers instead if file stream.
// https://gcc.gnu.org/onlinedocs/libstdc++/manual/streambufs.html

// Note: Bitmap file format uses Little-endian. This code does assume that
// the machine executing this code is using Little-endian as well.

// Bitmap File Header Version 3
struct BitmapFileHeader {
	uint16_t bfType;			// signature BM (0x42 0x4D)
	uint32_t bfSize;			// size of entire file (unreliable)
	uint32_t bfReserved;		// defaults to 0
	uint32_t bfOffBits;			// file offset to pixel array
};

// Bitmap Info Header Version 3
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

struct BitmapV2InfoHeader {
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
    uint32_t biRedMask;
    uint32_t biGreenMask;
    uint32_t biBlueMask;
};

struct BitmapV3InfoHeader {
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
    uint32_t biRedMask;
    uint32_t biGreenMask;
    uint32_t biBlueMask;
    uint32_t biAlphaMask;
};

#pragma pack(pop)
//#pragma pack()

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

	/////////////////////////////////////////////////
	/////// BmpEncoder
	/////////////////////////////////////////////////

    BmpEncoder::BmpEncoder()
        : flipVertically(true)
    {
        // Intentionally left empty
    }

    bool BmpEncoder::IsFlipVertically() const
    {
        return flipVertically;
    }

    void BmpEncoder::SetFlipVertically(bool flip)
    {
        flipVertically = flip;
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

	/////////////////////////////////////////////////
	/////// BmpEncoder
	/////////////////////////////////////////////////

	BmpDecoder::BmpDecoder()
	{
		// Intentionally left empty.
	}

	std::unique_ptr<Image> BmpDecoder::Decode(std::istream& is) const
	{
		// Read file header.
		BitmapFileHeader fh;

		is.read(reinterpret_cast<char*>(&fh), sizeof(BitmapFileHeader));

		if (!is.good() || is.gcount() != sizeof(BitmapFileHeader)) {
			throw std::runtime_error("unable to read BMP file header");
		}

		if (fh.bfType != 0x4d42) {
			throw std::runtime_error("unable to read BMP file, invalid header");
		}

		// Read info header.
        BitmapV3InfoHeader ih;

		is.read(reinterpret_cast<char*>(&ih), sizeof(ih.biSize));

		if (!is.good() || is.gcount() != sizeof(ih.biSize)) {
			throw std::runtime_error("unable to read reading BMP info header");
		}

        int ihVersion;
        switch (ih.biSize) {
            case sizeof(BitmapInfoHeader):
                ihVersion = BI_INFOHEADER_A;
                break;

            case sizeof(BitmapV2InfoHeader):
                ihVersion = BI_INFOHEADER_B;
                break;

            case sizeof(BitmapV3InfoHeader):
                ihVersion = BI_INFOHEADER_C;
                break;

            default:
                throw std::runtime_error("unsupported BMP format (unknown bitmap info header, header size = "
                                         + std::to_string(ih.biSize) + ")");
        }

		is.read(reinterpret_cast<char*>(&ih.biWidth), ih.biSize - sizeof(ih.biSize));

		if (!is.good() || is.gcount() != ih.biSize - sizeof(ih.biSize)) {
			throw std::runtime_error("unable to read reading BMP info header");
		}

		if (ih.biCompression != BI_RGB && ih.biCompression != BI_BITFIELDS) {
			throw std::runtime_error("unsupported BMP format");
		}

        if (ih.biBitCount != BYTES_PER_PIXEL * 8 && ih.biBitCount != 4 * 8) {
            throw std::runtime_error("unsupported BMP format");
        }

		size_t readSoFar = sizeof(BitmapFileHeader) + ih.biSize;
		is.ignore(fh.bfOffBits - readSoFar);

		bool flip = ih.biHeight >= 0;
		ih.biHeight = std::abs(ih.biHeight);

        int numBytes = ih.biBitCount / 8;

		// Each line must contain number of bytes dividable by four.
		auto numPadding = CalcNumPadding(ih.biWidth, numBytes);

		buffer.resize(ih.biWidth * numBytes + numPadding);

		// Read bitmap data.
		auto result = std::make_unique<Image>(ih.biWidth, ih.biHeight);
		for (int j = 0; j < ih.biHeight; ++j) {
			is.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
 
			if (!is.good() || is.gcount() != buffer.size()) {
				throw std::runtime_error("unable to read bitmap data");
			}
			unsigned char *ptr = buffer.data();
			for (int i = 0; i < ih.biWidth; ++i) {
				unsigned char red, green, blue, alpha;
				blue = *ptr++;
				green = *ptr++;
				red = *ptr++;
                if (ih.biBitCount == 32) {
                    alpha = *ptr++;
                } else {
                    alpha = 255;
                }
				result->SetPixel(i, flip ? result->GetHeight() - 1 - j : j, Color4d::CreateFromRgb(red, green, blue, alpha));
			}
		}

		return result;
	}

	std::unique_ptr<Image> BmpDecoder::Decode(const char *filename) const
	{
		// Open file.
        std::ifstream ifs(filename, std::ios::in | std::ios::binary);
        if (!ifs) {
            throw std::runtime_error(std::string("unable to open BMP file '") 
				+ filename + "' for reading");
        }

		auto result = Decode(ifs);
		ifs.close();
		return result;
	}
}