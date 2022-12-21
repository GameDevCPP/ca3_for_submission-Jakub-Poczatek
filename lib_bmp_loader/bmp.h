#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

#pragma pack(push, 1) //Specifies the packing alignment for struct, union, and class members.
struct BMPFileHeader{
    uint16_t fileType{0x4D42}; // File type set for BM which is 0x4D42.
    uint32_t fileSize{0}; // Size of the file (in bytes).
    [[maybe_unused]] uint16_t reservedOne{0}; // Reserved, always 0.
    [[maybe_unused]] uint16_t reservedTwo{0}; // Reserved, always 0.
    uint32_t offsetData{0}; //Start position of pixel data (bytes from the beginning of file).
};

struct BMPInfoHeader {
    uint32_t size{ 0 }; // Size of this header (in bytes).
    int32_t width{ 0 }; // Width of bitmap in pixels.
    int32_t height{ 0 }; // Height of bitmap in pixels.
    // If height is positive, reads bottom-up, with origin in lower left corner.
    // If height is negative, reads top-down, with origin in upper left corner.


    [[maybe_unused]] uint16_t planes{ 1 }; // # of planes for the target device, this is always 1.
    uint16_t bitCount{0}; // # of bits per pixel.
    [[maybe_unused]] uint32_t compression{0}; // 0 = uncompressed. Only uncompressed supported.
    [[maybe_unused]] uint32_t sizeImage{0}; // 0 for uncompressed images.
    [[maybe_unused]] int32_t xPixelsPerMeter{0};
    [[maybe_unused]] int32_t yPixelsPerMeter{0};
    [[maybe_unused]] uint32_t colors_used{0}; // # of color indexes in the color table. 0 = max number for bit_count.
    [[maybe_unused]] uint32_t colors_important{0}; // # of colors used for displaying the bitmap. 0 = all colors required.
};
#pragma pack(pop)

class BMP {
private:
    uint32_t _rowStride{0};
    BMPFileHeader _fileHeader;
    BMPInfoHeader _infoHeader;
    std::vector<uint8_t> _data;
    std::vector<sf::Color> _colouredPixels;

    uint32_t makeStrideAligned(uint32_t alignStride);
    void setColouredPixels();

public:
    BMP(const char *fname);
    void read(const char *fname);
    std::vector<sf::Color> getColouredPixels();
    std::vector<uint8_t> getData();
    sf::Color getColourAt(int pixelIndex);
    int32_t getWidth();
    int32_t getHeight();
};

// Reference:
// https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/