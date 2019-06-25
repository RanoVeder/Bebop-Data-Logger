#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <inttypes.h>



struct DataFrame
{
    char code;
    char data[64];
};

int main(int argc, char* argv[])
{
    if(argc != 3){
        std::cout << "Usage: txt_generator source_folder destination_folder" << std::endl;
        return 0;
    }
    std::string data_file_path = std::string(argv[1]) + "/" + "data_logging_raw";
    std::string destination_folder = std::string(argv[2]);



    auto data_file = std::fstream(data_file_path, std::fstream::in | std::fstream::binary);

    if(!data_file.good())
    {
        std::cout << "Could not open file 'data_logging_raw'" << std::endl;
        return 0;
    }

    mkdir(destination_folder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    std::string txtName = destination_folder + "/out" + " .txt";
    auto txt_file = std::fstream(txtName, std::fstream::out);


    //Same number of images, so safe to do!
    DataFrame df;
    while(!data_file.eof())
    {
        data_file.read((char *)&df, sizeof(DataFrame));
        if((int)df.code == 5)
        {
            txt_file << "-start frame-" << std::endl;
            txt_file << "accel_unscaled.x: " << (int32_t)(df.data[0]) << std::endl;
            txt_file << "accel_unscaled.y: " << (int32_t)(df.data[4]) << std::endl;
            txt_file << "accel_unscaled.z: " << (int32_t)(df.data[8]) << std::endl;
            txt_file << "gyro_unscaled.p: " << (int32_t)(df.data[12]) << std::endl;
            txt_file << "gyro_unscaled.q: " << (int32_t)(df.data[16]) << std::endl;
            txt_file << "gyro_unscaled.r: " << (int32_t)(df.data[20]) << std::endl;
            txt_file << "mag_unscaled.x: " << (int32_t)(df.data[24]) << std::endl;
            txt_file << "mag_unscaled.y: " << (int32_t)(df.data[28]) << std::endl;
            txt_file << "mag_unscaled.z: " << (int32_t)(df.data[32]) << std::endl;
            txt_file << "mag_unscaled.z: " << (int32_t)(df.data[36]) << std::endl;
            txt_file << "timestamp: " << (int64_t)(df.data[40]) << std::endl;
            txt_file << "-end frame-" << std::endl;
        }

    }


    data_file.close();

    
    return 1;
}