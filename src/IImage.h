/*
 Egor Bogolyubov
 student139974@student.spbu.ru
 LabWork1
*/

#ifndef IIMAGE_H
#define IIMAGE_H

#include <string>

class IImage
{
public:
    virtual ~IImage() = default;

    virtual void rotate90Clockwise() = 0;
    virtual void rotate90CounterClockwise() = 0;
    virtual void applyGaussianBlur() = 0;
    virtual void save(const std::string& filename) const = 0;
};

#endif
