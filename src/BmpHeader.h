/*
 Egor Bogolyubov
 student139974@student.spbu.ru
 LabWork1
*/

#ifndef BMPHEADER_H
#define BMPHEADER_H

#include <cstdint>
#include <istream>
#include <ostream>

class BmpHeader
{
public:
    BmpHeader();
    void readFrom(std::istream& is);
    void writeTo(std::ostream& os) const;
    void updateSize(uint32_t pixelDataSize, int width, int height);

    uint32_t getFileSize() const;
    uint32_t getPixelOffset() const;
    int getWidth() const
    {
        return width_;
    }
    int getHeight() const
    {
        return height_;
    }
    bool isTopDown() const
    {
        return originalBiHeight_ < 0;
    }
    int getOriginalHeight() const
    {
        return originalBiHeight_;
    }

    int getRowSize(int width) const;

private:
#pragma pack(push, 1)
    struct FileHeader
    {
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1;
        uint16_t bfReserved2;
        uint32_t bfOffBits;
    };

    struct InfoHeader40
    {
        uint32_t biSize;
        int32_t  biWidth;
        int32_t  biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        int32_t  biXPelsPerMeter;
        int32_t  biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };
#pragma pack(pop)

    FileHeader fileHeader;
    int width_ = 0;
    int height_ = 0;
    int originalBiHeight_ = 0;
    uint32_t biSizeImage_ = 0;
};

#endif
