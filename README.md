# LabWork1

## Author  
Bogolyubov Egor 25.581-MM

## Contacts  
st139974@student.spbu.ru

## Description  
BMP image processor supporting 90° rotations and Gaussian blur.  
Implements IImage interface, uses custom BmpHeader class.  
Works with 24-bit uncompressed BMP (including 124-byte headers).

## Build  
make test  
make demo    

## Run Tests  
./bmp_tests

## Process Custom Image  
./bmp_demo path/to/your/image.bmp

## Outputs:
assets/output_rotate_cw.bmp
assets/output_rotate_ccw.bmp
assets/output_blur.bmp

## Classes  
`IImage` (abstract interface)  
`BmpImage` (concrete implementation)  
`BmpHeader` (BMP header handling)

## Testing  
`BmpImageTest.cpp`
