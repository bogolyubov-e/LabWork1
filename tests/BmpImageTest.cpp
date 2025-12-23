/*
 Egor Bogolyubov
 student139974@student.spbu.ru
 LabWork1
*/

#include <gtest/gtest.h>
#include "BmpImage.h"
#include <filesystem>
#include <fstream>
#include <cstdint>

const std::string INPUT = "assets/input.bmp";
const std::string OUTPUT_CW = "assets/output_rotate_cw.bmp";
const std::string OUTPUT_CCW = "assets/output_rotate_ccw.bmp";
const std::string OUTPUT_BLUR = "assets/output_blur.bmp";
const std::string OUTPUT_SAVE1 = "assets/output_save_original.bmp";

bool isValidBmp(const std::string& path, int expectedWidth = -1, int expectedHeight = -1)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;

    char buffer[54];
    file.read(buffer, sizeof(buffer));
    if (!file) return false;

    if (buffer[0] != 'B' || buffer[1] != 'M') return false;

    int width = *reinterpret_cast<int32_t*>(buffer + 18);
    int height = *reinterpret_cast<int32_t*>(buffer + 22);
    height = std::abs(height);

    if (expectedWidth > 0 && width != expectedWidth) return false;
    if (expectedHeight > 0 && height != expectedHeight) return false;

    uint16_t bitCount = *reinterpret_cast<uint16_t*>(buffer + 28);
    uint32_t compression = *reinterpret_cast<uint32_t*>(buffer + 30);
    return (bitCount == 24) && (compression == 0);
}

TEST(BmpImageTest, Save_OriginalImagePreserved)
{
    ASSERT_TRUE(std::filesystem::exists(INPUT));
    BmpImage img(INPUT);
    img.save(OUTPUT_SAVE1);
    EXPECT_TRUE(std::filesystem::exists(OUTPUT_SAVE1));
    EXPECT_TRUE(isValidBmp(OUTPUT_SAVE1, img.width(), img.height()));
}

TEST(BmpImageTest, Save_ReopenedImageHasSameSize)
{
    BmpImage original(INPUT);
    original.save(OUTPUT_SAVE1);
    BmpImage reopened(OUTPUT_SAVE1);
    EXPECT_EQ(reopened.width(), original.width());
    EXPECT_EQ(reopened.height(), original.height());
}

TEST(BmpImageTest, Rotate90Clockwise_ChangesDimensionsCorrectly)
{
    BmpImage img(INPUT);
    int w = img.width(), h = img.height();
    img.rotate90Clockwise();
    EXPECT_EQ(img.width(), h);
    EXPECT_EQ(img.height(), w);
}

TEST(BmpImageTest, Rotate90Clockwise_SavedFileIsValid)
{
    BmpImage img(INPUT);
    int w = img.width(), h = img.height();
    img.rotate90Clockwise();
    img.save(OUTPUT_CW);
    EXPECT_TRUE(std::filesystem::exists(OUTPUT_CW));
    EXPECT_TRUE(isValidBmp(OUTPUT_CW, h, w));
}

TEST(BmpImageTest, Rotate90CounterClockwise_ChangesDimensionsCorrectly)
{
    BmpImage img(INPUT);
    int w = img.width(), h = img.height();
    img.rotate90CounterClockwise();
    EXPECT_EQ(img.width(), h);
    EXPECT_EQ(img.height(), w);
}

TEST(BmpImageTest, Rotate90CounterClockwise_SavedFileIsValid)
{
    BmpImage img(INPUT);
    int w = img.width(), h = img.height();
    img.rotate90CounterClockwise();
    img.save(OUTPUT_CCW);
    EXPECT_TRUE(std::filesystem::exists(OUTPUT_CCW));
    EXPECT_TRUE(isValidBmp(OUTPUT_CCW, h, w));
}

TEST(BmpImageTest, ApplyGaussianBlur_DoesNotCrash)
{
    BmpImage img(INPUT);
    img.applyGaussianBlur();
    img.save(OUTPUT_BLUR);
    EXPECT_TRUE(std::filesystem::exists(OUTPUT_BLUR));
    EXPECT_TRUE(isValidBmp(OUTPUT_BLUR, img.width(), img.height()));
}

TEST(BmpImageTest, RotateThenBlur_ProducesValidOutput)
{
    BmpImage img(INPUT);
    img.rotate90Clockwise();
    img.applyGaussianBlur();
    const std::string temp = "assets/temp_rot_blur.bmp";
    img.save(temp);
    EXPECT_TRUE(std::filesystem::exists(temp));
    EXPECT_TRUE(isValidBmp(temp, img.width(), img.height()));
    std::filesystem::remove(temp);
}
