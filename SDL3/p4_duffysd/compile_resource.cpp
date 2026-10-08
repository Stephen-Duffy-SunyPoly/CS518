#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

int main(int argc, char *argv[]) {
    std::vector<std::string> args(argv, argv + argc);

    if (argc < 2) {
        std::cerr << "Usage: " << args[0] << " <file list>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ofstream headerFile("assets.h");
    //header guard
    headerFile << "#ifndef ASSETS_H" << std::endl;
    headerFile << "#define ASSETS_H" << std::endl;
    //c linkage
    headerFile << "#ifdef __cplusplus" << std::endl;
    headerFile << "extern \"C\" {" << std::endl;
    headerFile << "#endif" << std::endl<<std::endl;


    std::ofstream dataFile("assets.c");
    dataFile << "#include \"assets.h\"" << std::endl;

    for (int i = 1; i < argc; i++) {
        std::string fileName = std::filesystem::path(args[i]).filename().string();
        if (fileName[0] == '0' || fileName[0] == '1' || fileName[0] == '2' || fileName[0] == '3' || fileName[0] == '4' || fileName[0] == '5' || fileName[0] == '6' || fileName[0] == '7' || fileName[0] == '8' || fileName[0] == '9') {
            fileName = "_" + fileName; // NOLINT(*-inefficient-string-concatenation)
        }
        for (int j = 0; j < fileName.length(); j++) {
            if (fileName[j] == '.' || fileName[j] == ' ') {
                fileName[j] = '_';
            }
        }
        std::ifstream assetFile(args[i], std::ios::binary);
        if (!assetFile.is_open()) {
            std::cerr << "Could not open file " << args[i] << std::endl;
            continue;
        }

        uint64_t bytesWritten = std::filesystem::file_size(args[i]);
        dataFile << "const char "<<fileName<<"_data[] = {";
        dataFile << std::hex;
        for (uint64_t j = 0; j < bytesWritten; j++) {
            auto byte = static_cast<uint8_t>(assetFile.get());
            if (byte == 0) {
                dataFile <<"0";
            } else {
                dataFile <<"0x"<< static_cast<int>(byte);
            }
            if (j != (bytesWritten - 1)) {
                dataFile << ", ";
            }
        }
        dataFile << "};" << std::endl;
        //wright the size
        headerFile << "const int "<<fileName<<"_size = " << bytesWritten << ";" << std::endl;
        headerFile << "extern const char "<<fileName<<"_data[];" << std::endl;

        assetFile.close();
    }

    //close the c linkage
    headerFile << std::endl << "#ifdef __cplusplus" << std::endl;
    headerFile << "}" << std::endl;
    headerFile << "#endif" << std::endl;

    //close the header guard
    headerFile << "#endif" << std::endl;
    headerFile.close();
    dataFile.close();

    return EXIT_SUCCESS;
}