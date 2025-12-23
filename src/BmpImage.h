/*
 Egor Bogolyubov
 student139974@student.spbu.ru
 LabWork1
*/

#ifndef BMPIMAGE_H
#define BMPIMAGE_H

#include "IImage.h"
#include "BmpHeader.h"
#include <string>
#include <vector>

class BmpImage : public IImage
{
public:
    explicit BmpImage(const std::string& filename);
    ~BmpImage() override = default;

    void rotate90Clockwise() override;
    void rotate90CounterClockwise() override;
    void applyGaussianBlur() override;
    void save(const std::string& filename) const override;

    int width() const
    {
        return header.getWidth();
    }
    int height() const
    {
        return header.getHeight();
    }

private:
    BmpHeader header;
    std::vector<unsigned char> pixels;

    static void flipVertically(std::vector<unsigned char>& data, int width, int height);
};

#endif
