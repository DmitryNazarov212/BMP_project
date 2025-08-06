#include <iostream>
#include <filesystem>
#include "BMP_file.hpp"
#include <string>
#include <exception>
int main()
{
    BmpHeader header;

    std::string fileName;

    try{
        std:: cout << "Enter input BPM file name: ";
        std::cin >> fileName;
        FileBMP file(std::filesystem::current_path().parent_path()/"image/"/fileName); 

        if(file.readImage(file.getFileName()))
            std::cout << "Source image added!" << std::endl;
        file.showImage();
        
        if(file.editImage())
            std::cout << "Image has been edited!" << std::endl;

        file.showImage();
        std:: cout << "Enter file name to save it: ";
        std::cin >> fileName;
        file.saveImage(fileName);
    }
    catch(std::exception &e){
        std::cout << "Error: " << e.what() << std:: endl;
    }
    catch(...)
    {
        std::cout << "Error: Unrecognised problem" << std:: endl; 
    }

    return 0;
}