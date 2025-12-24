/*
 Egor Bogolyubov
 student139974@student.spbu.ru
 LabWork1
*/

#include "BmpImage.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_image.bmp>\n";
        return 1;
    }

    std::string input = argv[1];
    try
    {
        {
            BmpImage img(input);
            img.applyGaussianBlur();
            img.save("assets/output_blur.bmp");
            std::cout << "Saved: assets/output_blur.bmp\n";
        }

        {
            BmpImage img(input);
            img.rotate90Clockwise();
            img.save("assets/output_rotate_cw.bmp");
            std::cout << "Saved: assets/output_rotate_cw.bmp\n";
        }

        {
            BmpImage img(input);
            img.rotate90CounterClockwise();
            img.save("assets/output_rotate_ccw.bmp");
            std::cout << "Saved: assets/output_rotate_ccw.bmp\n";
        }

        std::cout << "All operations completed successfully.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
