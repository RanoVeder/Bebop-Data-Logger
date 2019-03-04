#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

#define IMAGE_WIDTH 1280
#define IMAGE_HEIGHT 720

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct DataFrame
{
    uint64_t time;
    char data[IMAGE_WIDTH*IMAGE_HEIGHT];
};

int main(int argc, char* argv[])
{
    if(argc != 3){
        std::cout << "Usage: image_extractor source_folder destination_folder" << std::endl;
        return 0;
    }
    std::string left_file_path = std::string(argv[1]) + "/" + "raw_data_left_camera";
    std::string right_file_path = std::string(argv[1]) + "/" + "raw_data_right_camera";
    std::string destination_folder = std::string(argv[2]);



    auto left_file = std::fstream(left_file_path, std::fstream::in | std::fstream::binary);
    auto right_file = std::fstream(right_file_path, std::fstream::in | std::fstream::binary);

    if(!left_file.good() || !right_file.good())
    {
        std::cout << "Could not open file 'raw_data_left_camera' or 'raw_data_right_camera'" << std::endl;
        return 0;
    }

    mkdir(destination_folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);


    //Same number of images, so safe to do!
    DataFrame df;
    while(!left_file.eof())
    {
        left_file.read((char *)&df, sizeof(DataFrame));
        std::string leftName = destination_folder + "/L_" + std::to_string(df.time) + ".png";
        stbi_write_png(leftName.c_str(), IMAGE_WIDTH, IMAGE_HEIGHT, 1, &df.data, 1280);

        right_file.read((char *)&df, sizeof(DataFrame));
        std::string rightName = destination_folder + "/R_" + std::to_string(df.time) + ".png";
        stbi_write_png(rightName.c_str(), IMAGE_WIDTH, IMAGE_HEIGHT, 1, &df.data, 1280);
    }


    left_file.close();
    right_file.close();

    
    return 1;
}