#pragma once
#include <string>
#include <mutex>
#include <vector>


#pragma pack(push, 1)
struct BmpHeader {
    char bitmapSignatureBytes[2];
    uint32_t sizeOfBitmapFile;
    uint32_t reservedBytes;
    uint32_t pixelDataOffset;
};
#pragma pack(pop)


struct BmpInfoHeader {
    uint32_t sizeOfThisHeader;
    int32_t width; 
    int32_t height;
    uint16_t numberOfColorPlanes;
    uint16_t colorDepth;
    uint32_t compressionMethod;
    uint32_t rawBitmapDataSize; 
    int32_t horizontalResolution; 
    int32_t verticalResolution;
    uint32_t colorTableEntries;
    uint32_t importantColors;
};


class FileBMP
{
private:
    BmpHeader header;
    BmpInfoHeader infoHeader;
    std::vector<uint8_t> pixelData;
    const std::string FileName_;

    int bytesPerPixel;
    int rowPadding;
    int32_t width_; 
    int32_t height_;

    void drawLine(int x1, int y1, int x2, int y2);
    const uint8_t* getPixel(int x, int y) const;
    uint8_t* getPixel(int x, int y);

public:
    explicit FileBMP(const std::string FileName) : FileName_(FileName) {};

    void readImage(const std::string FileName_);

    
    void editImage();

    void saveImage(std::string fileName);

    void showImage() const;

    ~FileBMP() {};

    std::string getFileName() {return FileName_; };
};
