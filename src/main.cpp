#include <iostream>
#include "BMP_file.hpp"

int main()
{
    BmpHeader header;

    FileBMP file("/home/dmitry/Pictures/bmp_24.bmp");
    
    file.readImage(file.getFileName());

    file.showPicture();

    file.editImage();

    file.showPicture();
}