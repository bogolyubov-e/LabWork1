/*
 Egor Bogolyubov
 student139974@student.spbu.ru
 LabWork1
*/

#include "BmpHeader.h"
#include <stdexcept>
#include <cstring>

BmpHeader::BmpHeader()
{
    fileHeader.bfType = 0x4D42;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
}

void BmpHeader::readFrom(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (fileHeader.bfType != 0x4D42)
    {
        throw std::runtime_error("Invalid BMP signature (not 'BM')");
    }

    uint32_t biSize;
    is.read(reinterpret_cast<char*>(&biSize), sizeof(biSize));
    if (biSize < 40)
    {
        throw std::runtime_error("Unsupported BMP header size (< 40 bytes)");
    }

    char rest[36];
    is.read(rest, 36);

    int32_t biWidth, biHeight;
    uint16_t biPlanes, biBitCount;
    uint32_t biCompression, biSizeImage;
    std::memcpy(&biWidth, rest, 4);
    std::memcpy(&biHeight, rest + 4, 4);
    std::memcpy(&biPlanes, rest + 8, 2);
    std::memcpy(&biBitCount, rest + 10, 2);
    std::memcpy(&biCompression, rest + 12, 4);
    std::memcpy(&biSizeImage, rest + 16, 4);

    if (biPlanes != 1 || biBitCount != 24 || biCompression != 0)
    {
        throw std::runtime_error("Only 24-bit uncompressed BMP is supported");
    }

    originalBiHeight_ = biHeight;
    width_ = biWidth;
    height_ = std::abs(biHeight);
    biSizeImage_ = biSizeImage;

    if (biSize > 40)
    {
        is.ignore(biSize - 40);
    }
}

void BmpHeader::writeTo(std::ostream& os) const
{
    FileHeader outFH = fileHeader;
    InfoHeader40 outIH{};
    outIH.biSize = 40;
    outIH.biWidth = width_;
    outIH.biHeight = height_;
    outIH.biPlanes = 1;
    outIH.biBitCount = 24;
    outIH.biCompression = 0;
    outIH.biSizeImage = biSizeImage_;
    outIH.biXPelsPerMeter = 0;
    outIH.biYPelsPerMeter = 0;
    outIH.biClrUsed = 0;
    outIH.biClrImportant = 0;

    outFH.bfSize = sizeof(outFH) + sizeof(outIH) + biSizeImage_;
    outFH.bfOffBits = sizeof(outFH) + sizeof(outIH);

    os.write(reinterpret_cast<const char*>(&outFH), sizeof(outFH));
    os.write(reinterpret_cast<const char*>(&outIH), sizeof(outIH));
}

void BmpHeader::updateSize(uint32_t pixelDataSize, int width, int height)
{
    width_ = width;
    height_ = height;
    biSizeImage_ = pixelDataSize;
}

uint32_t BmpHeader::getFileSize() const
{
    return fileHeader.bfSize;
}

uint32_t BmpHeader::getPixelOffset() const
{
    return fileHeader.bfOffBits;
}

int BmpHeader::getRowSize(int width) const
{
    return (width * 3 + 3) & ~3;
}
