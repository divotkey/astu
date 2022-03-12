/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "HdrCodec.h"
#include "Graphics/Image.h"

// C++ Standard Library includes
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <string>

using namespace std;

namespace astu {

    const std::array<unsigned char, 11> HdrDecoder::MagicNumber
        = {0x23, 0x3f, 0x52, 0x41, 0x44, 0x49, 0x41, 0x4e, 0x43, 0x45, 0x0a};

    HdrDecoder::HdrDecoder() {
        // Intentionally left empty.
    }

    std::unique_ptr<Image> HdrDecoder::Decode(std::istream &is) const {
        // Check magic number

        for (unsigned char magic : MagicNumber) {
            int ch = is.get();
            if (ch == std::char_traits<char16_t>::eof() || ch != magic) {
                throw std::runtime_error("unable to read HDR file, invalid header");
            }
        }

        std::string line;
        getline(is, line);
        // ignore this line.
        if (!is.good()) {
            throw std::runtime_error("unable to read HDR file, invalid header");
        }

        // read blank line.
        line.clear();
        getline(is, line);
        if (!is.good() || !line.empty()) {
            throw std::runtime_error("unable to read HDR file, invalid header");
        }

        // read resolution
        line.clear();
        getline(is, line);
        cout << "resolution = " << line << endl;
        size_t pos = 0;
        ReadText(line, pos, "-Y ");
        int height = ReadInt(line, pos);
        ReadText(line, pos, "+X ");
        int width = ReadInt(line, pos);

        auto result = make_unique<Image>(width, height);
        //cout << "reading HDR image " << width << " x " << height << endl;
        for (int j = 0; j < height; ++j) {
            //cout << "processing line " << j << endl;
            unsigned char rgbe[4];
            is.read(reinterpret_cast<char*>(rgbe), 4);
            if (!is.good()) {
                throw std::runtime_error("unable to read HDR file, invalid data");
            }

            //assert(rgbe[0] == 2 && rgbe[1] == 2 && !(rgbe[2] & 0x80));

            buffer.resize(4 * width);
            unsigned char* ptr = buffer.data();
            for (int i = 0; i < 4; ++i) {
                unsigned char *ptrEnd = buffer.data() + (i + 1) * width;
                while (ptr < ptrEnd) {
                    unsigned char buf[2];
                    is.read(reinterpret_cast<char*>(buf), 2);
                    if (buf[0] > 128) {
                        int count = buf[0] - 128;
                        if (count == 0 || count > ptrEnd - ptr) {
                            throw std::runtime_error("unable to read HDR file, invalid data");
                        }
                        while (count--) {
                            *ptr++ = buf[1];
                        }
                    } else {
                        int count = buf[0];
                        if (count == 0 || count > ptrEnd - ptr) {
                            throw std::runtime_error("unable to read HDR file, invalid data");
                        }
                        *ptr++ = buf[1];
                        if (--count) {
                            is.read(reinterpret_cast<char*>(ptr), count);
                            if (!is.good()) {
                                throw std::runtime_error("unable to read HDR file, invalid data");
                            }
                            ptr += count;
                        }
                    }
                }
                for (int i = 0; i < width; ++i) {
                    rgbe[0] = buffer[i];
                    rgbe[1] = buffer[i + width];
                    rgbe[2] = buffer[i + width * 2];
                    rgbe[3] = buffer[i + width * 3];
                    float r, g, b;
                    RgbeToFloat(rgbe, r, g, b);
                    //if (r > 1 || g > 1 || b > 1) {
                    //    //cout << "hudriwudri" << endl;
                    //    cout << "r = " << r << ", " << g << ", " << b << endl;
                    //}
                    Color4d c(r, g, b, 1.0);
                    result->SetPixel(i, j, Color4d(r, g, b, 1.0));
                }
            }

            //for (int i = 0; i < width; ++i) {
            //    float r, g, b;
            //    RgbeToFloat(reinterpret_cast<unsigned char*>(rgbe), r, g, b);
            //    result->SetPixel(i, j, Color4d(r, g, b));
            //}
        }
        return result;
    }

    std::unique_ptr<Image> HdrDecoder::Decode(const char *filename) const {
        // Open file.
        std::ifstream ifs(filename, std::ios::in | std::ios::binary);
        if (!ifs) {
            throw std::runtime_error(std::string("unable to open HDR file '")
                                     + filename + "' for reading");
        }

        auto result = Decode(ifs);
        ifs.close();
        return result;
    }

    void HdrDecoder::ReadText(const string &source, size_t &inOutPos, const string &text) const {
        for (size_t i = 0; i < text.size(); ++i) {
            if (inOutPos >= source.size() || source[inOutPos] != text[i]) {
                throw std::runtime_error("unable to read HDR file, invalid header");
            }
            ++inOutPos;
        }
    }

    int HdrDecoder::ReadInt(const string &source, size_t &inOutPos) const {
        if (inOutPos >= source.size()) {
            throw std::runtime_error("unable to read HDR file, invalid header");
        }


        int ch = source[inOutPos++];

        int result = 0;
        while (isdigit(ch)) {
            result *= 10;
            result += (ch - '0');
            if (inOutPos >= source.size()) {
                break;
            }
            ch = source[inOutPos++];
        }

        return result;
    }

    void HdrDecoder::RgbeToFloat(unsigned char *rgbe, float &outRed, float &outGreen, float &outBlue) const {
        if (rgbe[3]) {
            // nonzero pixel
            float f = static_cast<float>(ldexp(1.0, rgbe[3]-(int)(128+8)));
            outRed = rgbe[0] * f;
            outGreen = rgbe[1] * f;
            outBlue = rgbe[2] * f;
        } else {
            outRed = outGreen = outBlue = 0.0;
        }
    }

}
