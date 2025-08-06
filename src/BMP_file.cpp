#include "BMP_file.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <cmath>

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


bool FileBMP::readImage(const std::string FileName_)
{
    std::ifstream file(FileName_, std::ios::binary);

    if(!file.is_open())
    {
        throw std::runtime_error("Cannot read file");
    }
    file.read((char*)&header, sizeof(header));
    if(header.bitmapSignatureBytes[0] != 'B' || header.bitmapSignatureBytes[1] != 'M')
    {
        throw std::runtime_error("Signuture is unrecognized");
    }
    file.read((char*)&infoHeader, sizeof(infoHeader));

    if(infoHeader.colorDepth != 32 && infoHeader.colorDepth != 24) {
        throw std::runtime_error("Color depth only 32 and 24 allowed");       
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
        pixelData.clear(); 
        throw std::runtime_error("Error reading pixel data");
    }
    return true;
}


void FileBMP::showImage() const {
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


bool FileBMP::editImage()
{   
    drawLine(2, 5, width_ - 18, height_ - 5);
    drawLine(2, height_ - 5, width_ - 18, 5);
    return true;
}



void FileBMP::drawLine(int x1, int y1, int x2, int y2) {
    bool steep = std::abs(y2 - y1) > std::abs(x2 - x1); 
    if(steep)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    if(x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    
    int dx = x2 - x1;
    int dy = std::abs(y2 - y1);
    int error = dx / 2;
    int ystep = (y1 < y2) ? 1 : -1;
    int y = y1;

    for(int x = x1; x <= x2; ++x)
    {
        if (steep) {
            auto pixel = getPixel(y, x);
            if (pixel) {
                pixel[0] = 0;
                pixel[1] = 0;
                pixel[2] = 0;
            }
        } else {
            auto pixel = getPixel(x, y);
            if (pixel) {
                pixel[0] = 0;
                pixel[1] = 0;
                pixel[2] = 0;
            }
        }

        error -= dy;
        if(error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
    
}

void FileBMP::saveImage(std::string fileName)
{
    std::ofstream file(std::filesystem::current_path().parent_path()/"image/"/fileName, std::ios::binary);

    if(!file.is_open())
    {
        throw std::runtime_error("Cannot open");
    }
    file.write((char*) &header, sizeof(header));
    file.write((char*) &infoHeader, sizeof(infoHeader));

    file.seekp(header.pixelDataOffset, std::ios::beg);
    file.write((char*) pixelData.data(), pixelData.size());
}
