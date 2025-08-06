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
        std::cin >> fileName;
        FileBMP file(std::filesystem::current_path().parent_path()/"image/"/fileName); 
        file.readImage(file.getFileName());
        file.showImage();
        file.editImage();

        file.showImage();

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