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

constexpr uint16_t BMP_SIGNATURE      = 0x4D42;  
constexpr uint16_t BITS_PER_PIXEL     = 24;      
constexpr uint32_t COMPRESSION_NONE   = 0;
constexpr uint16_t BMP_PLANES         = 1;
constexpr int BMP_ALIGNMENT           = 4;     

constexpr int BYTES_PER_PIXEL = BITS_PER_PIXEL / 8; 

constexpr int FILE_HEADER_SIZE = 14;   
constexpr int INFO_HEADER_SIZE = 40;   

constexpr int OFFSET_BI_WIDTH       = 0;
constexpr int OFFSET_BI_HEIGHT      = OFFSET_BI_WIDTH + 4;
constexpr int OFFSET_BI_PLANES      = OFFSET_BI_HEIGHT + 4;
constexpr int OFFSET_BI_BIT_COUNT   = OFFSET_BI_PLANES + 2;
constexpr int OFFSET_BI_COMPRESSION = OFFSET_BI_BIT_COUNT + 2;
constexpr int OFFSET_BI_SIZE_IMAGE  = OFFSET_BI_COMPRESSION + 4;

constexpr int OFFSET_FILE_BI_WIDTH    = FILE_HEADER_SIZE + 4 + OFFSET_BI_WIDTH;      
constexpr int OFFSET_FILE_BI_HEIGHT   = FILE_HEADER_SIZE + 4 + OFFSET_BI_HEIGHT;     
constexpr int OFFSET_FILE_BI_BIT_COUNT= FILE_HEADER_SIZE + 4 + OFFSET_BI_BIT_COUNT;  
constexpr int OFFSET_FILE_BI_COMPRESSION = FILE_HEADER_SIZE + 4 + OFFSET_BI_COMPRESSION; 

class BmpHeader
{
public:
    BmpHeader();
    void readFrom(std::istream& is);
    void writeTo(std::ostream& os) const;
    void updateSize(uint32_t pixelDataSize, int width, int height);

    uint32_t getFileSize() const;
    uint32_t getPixelOffset() const;
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    bool isTopDown() const { return originalBiHeight_ < 0; }

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
