#include "BMP_file.hpp"
#include <iostream>
#include <fstream>

const uint8_t* FileBMP::getPixel(int x, int y) const {
    size_t offset = (height_ - 1 - y) * (width_ * bytesPerPixel + rowPadding) + x * bytesPerPixel;
    return pixelData.data() + offset;
}


uint8_t* FileBMP::getPixel(int x, int y) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        std::cerr << "Invalid pixel coordinates: (" << x << ", " << y << ")" << std::endl;
        return nullptr;
    }
    size_t offset = (height_ - 1 - y) * (width_ * bytesPerPixel + rowPadding) + x * bytesPerPixel;
    return pixelData.data() + offset;
}


void FileBMP::readImage(const std::string FileName_)
{
    std::ifstream file(FileName_, std::ios::binary);

    if(!file.is_open())
    {
        std::cerr << "Cannot read file" << std::endl;
        return;
    }
    
    file.read((char*)&header, sizeof(header));
    if(header.bitmapSignatureBytes[0] != 'B' || header.bitmapSignatureBytes[1] != 'M')
    {
        std::cerr << "Signuture is unrecognized" << std::endl;
        return;
    }
    file.read((char*)&infoHeader, sizeof(infoHeader));

    if(infoHeader.colorDepth != 32 && infoHeader.colorDepth != 24) {
        std::cerr << "Color depth only 32 and 24 allowed" << std::endl;
        return;       
    }

    width_ = infoHeader.width;
    height_ = infoHeader.height;
    bytesPerPixel = infoHeader.colorDepth / 8;

    rowPadding = (4 - (width_ * bytesPerPixel) % 4);

    const size_t pixelDataSize = height_ * (width_ * bytesPerPixel + rowPadding);
    pixelData.resize(pixelDataSize); 

    file.seekg(header.pixelDataOffset, std::ios::beg);

    file.read((char*)pixelData.data(), pixelDataSize);

    if (!file) {
        std::cerr << "Error reading pixel data" << std::endl;
        pixelData.clear(); 
    }
}


void FileBMP::showPicture() const {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            const auto pixel = getPixel(x, y);
            if (pixel[2] == 255 && pixel[1] == 255 && pixel[0] == 255) {
                std::cout << ".";
            } else {
                std::cout << "#";
            }
        }
        std::cout << std::endl;
    }
}


void FileBMP::editImage()
{   
    drawLine(2, 5, width_ - 18, height_ - 5);
    drawLine(2, height_ - 5, width_ - 18, 5);
}



void FileBMP::drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        auto pixel = getPixel(x1, y1);
        if (pixel) { 
            pixel[0] = 0; 
            pixel[1] = 0; 
            pixel[2] = 0; 
        }

        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}
