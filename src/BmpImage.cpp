/*
 Egor Bogolyubov
 student139974@student.spbu.ru
 LabWork1
*/

#include "BmpImage.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <climits>

constexpr int GAUSSIAN_KERNEL_SUM = 16;
constexpr int GAUSSIAN_KERNEL_SIZE = 3;

void BmpImage::flipVertically(std::vector<unsigned char>& data, int width, int height)
{
    constexpr int alignment = BMP_ALIGNMENT;
    int rowSize = (width * BYTES_PER_PIXEL + alignment - 1) & ~(alignment - 1);
    for (int y = 0; y < height / 2; ++y)
    {
        unsigned char* row1 = data.data() + y * rowSize;
        unsigned char* row2 = data.data() + (height - 1 - y) * rowSize;
        std::swap_ranges(row1, row1 + rowSize, row2);
    }
}

BmpImage::BmpImage(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    header.readFrom(file);
    int w = header.getWidth();
    int h = header.getHeight();

    if (w <= 0 || h <= 0 || w > 10000 || h > 10000)
    {
        throw std::runtime_error("Invalid image dimensions");
    }

    int rowSize = header.getRowSize(w);
    size_t dataSize = static_cast<size_t>(rowSize) * h;
    if (dataSize > 500'000'000)
    {
        throw std::runtime_error("Image too large");
    }

    pixels.resize(dataSize);
    file.seekg(header.getPixelOffset());
    file.read(reinterpret_cast<char*>(pixels.data()), dataSize);
    if (!file)
    {
        throw std::runtime_error("Failed to read pixel data");
    }

    if (!header.isTopDown())
    {
        flipVertically(pixels, w, h);
    }
}

void BmpImage::rotate90Clockwise()
{
    rotate90CounterClockwise();
    rotate90CounterClockwise();
    rotate90CounterClockwise();
}


void BmpImage::rotate90CounterClockwise()
{
    int oldW = header.getWidth();
    int oldH = header.getHeight();
    int newRowSize = header.getRowSize(oldH);
    std::vector<unsigned char> newPixels(newRowSize * oldW, 0);

    int oldRowSize = header.getRowSize(oldW);
    for (int y = 0; y < oldH; ++y)
    {
        for (int x = 0; x < oldW; ++x)
        {
            int srcIdx = y * oldRowSize + x * BYTES_PER_PIXEL;
            int dstIdx = (oldW - 1 - x) * newRowSize + y * BYTES_PER_PIXEL;
            newPixels[dstIdx + 0] = pixels[srcIdx + 0];
            newPixels[dstIdx + 1] = pixels[srcIdx + 1];
            newPixels[dstIdx + 2] = pixels[srcIdx + 2];
        }
    }

    header.updateSize(newRowSize * oldW, oldH, oldW);
    pixels = std::move(newPixels);
}

void BmpImage::applyGaussianBlur()
{
    if (header.getWidth() < GAUSSIAN_KERNEL_SIZE || header.getHeight() < GAUSSIAN_KERNEL_SIZE) return;

    std::vector<unsigned char> blurred = pixels;
    int w = header.getWidth();
    int h = header.getHeight();
    int rowSize = header.getRowSize(w);
    const int kernel[3][3] = {{1,2,1}, {2,4,2}, {1,2,1}};

    for (int y = 1; y < h - 1; ++y)
    {
        for (int x = 1; x < w - 1; ++x)
        {
            int b = 0, g = 0, r = 0;
            for (int dy = -1; dy <= 1; ++dy)
            {
                for (int dx = -1; dx <= 1; ++dx)
                {
                    int py = y + dy;
                    int px = x + dx;
                    int srcIdx = py * rowSize + px * BYTES_PER_PIXEL;
                    int k = kernel[dy + 1][dx + 1];
                    b += k * static_cast<int>(pixels[srcIdx + 0]);
                    g += k * static_cast<int>(pixels[srcIdx + 1]);
                    r += k * static_cast<int>(pixels[srcIdx + 2]);
                }
            }
            int dstIdx = y * rowSize + x * BYTES_PER_PIXEL;
            blurred[dstIdx + 0] = static_cast<unsigned char>(
                                      std::max(0, std::min(255, (b + GAUSSIAN_KERNEL_SUM / 2) / GAUSSIAN_KERNEL_SUM))
                                  );
            blurred[dstIdx + 1] = static_cast<unsigned char>(
                                      std::max(0, std::min(255, (g + GAUSSIAN_KERNEL_SUM / 2) / GAUSSIAN_KERNEL_SUM))
                                  );
            blurred[dstIdx + 2] = static_cast<unsigned char>(
                                      std::max(0, std::min(255, (r + GAUSSIAN_KERNEL_SUM / 2) / GAUSSIAN_KERNEL_SUM))
                                  );
        }
    }

    pixels = std::move(blurred);
}

void BmpImage::save(const std::string& filename) const
{
    std::vector<unsigned char> bottomUpPixels = pixels;
    flipVertically(bottomUpPixels, header.getWidth(), header.getHeight());

    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Cannot create file: " + filename);
    }
    header.writeTo(file);
    file.write(reinterpret_cast<const char*>(bottomUpPixels.data()), bottomUpPixels.size());
}
