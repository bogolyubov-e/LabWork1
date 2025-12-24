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
    fileHeader.bfType = BMP_SIGNATURE;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
}

void BmpHeader::readFrom(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if (fileHeader.bfType != BMP_SIGNATURE)
    {
        throw std::runtime_error("Invalid BMP signature (not 'BM')");
    }

    uint32_t biSize;
    is.read(reinterpret_cast<char*>(&biSize), sizeof(biSize));
    if (biSize < INFO_HEADER_SIZE)
    {
        throw std::runtime_error("Unsupported BMP header size (< 40 bytes)");
    }

    char rest[INFO_HEADER_SIZE - 4]; // 40 - 4 = 36
    is.read(rest, sizeof(rest));

    int32_t biWidth, biHeight;
    uint16_t biPlanes, biBitCount;
    uint32_t biCompression, biSizeImage;
    std::memcpy(&biWidth, rest + OFFSET_BI_WIDTH, sizeof(biWidth));
    std::memcpy(&biHeight, rest + OFFSET_BI_HEIGHT, sizeof(biHeight));
    std::memcpy(&biPlanes, rest + OFFSET_BI_PLANES, sizeof(biPlanes));
    std::memcpy(&biBitCount, rest + OFFSET_BI_BIT_COUNT, sizeof(biBitCount));
    std::memcpy(&biCompression, rest + OFFSET_BI_COMPRESSION, sizeof(biCompression));
    std::memcpy(&biSizeImage, rest + OFFSET_BI_SIZE_IMAGE, sizeof(biSizeImage));

    if (biPlanes != BMP_PLANES || biBitCount != BITS_PER_PIXEL || biCompression != COMPRESSION_NONE)
    {
        throw std::runtime_error("Only 24-bit uncompressed BMP is supported");
    }

    originalBiHeight_ = biHeight;
    width_ = biWidth;
    height_ = std::abs(biHeight);
    biSizeImage_ = biSizeImage;

    if (biSize > INFO_HEADER_SIZE)
    {
        is.ignore(biSize - INFO_HEADER_SIZE);
    }
}

void BmpHeader::writeTo(std::ostream& os) const
{
    FileHeader outFH = fileHeader;
    InfoHeader40 outIH{};
    outIH.biSize = INFO_HEADER_SIZE;
    outIH.biWidth = width_;
    outIH.biHeight = height_;
    outIH.biPlanes = BMP_PLANES;
    outIH.biBitCount = BITS_PER_PIXEL;
    outIH.biCompression = COMPRESSION_NONE;
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
    constexpr int alignment = BMP_ALIGNMENT;
    int pixelDataSize = width * BYTES_PER_PIXEL;
    return (pixelDataSize + alignment - 1) & ~(alignment - 1);
}
