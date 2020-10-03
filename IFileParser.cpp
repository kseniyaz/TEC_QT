#include "IFileParser.h"

inline std::array<double, 73> parseParamsBlockI(std::array<std::string, 6>& block){
    std::array<double, 73> params;

    std::size_t i=0;
    std::istringstream valuesStream(block[1]);
    for(auto &value: params){
        double temp;
        valuesStream>>temp;
        value = temp/10;
        i++;
        if(i % 16 == 0)
            valuesStream = std::istringstream(block[i/16 + 1]);
    }
    return params;
}

std::vector<IFileData> parseIFile(std::ifstream* file){
    std::vector<IFileData> fileData;

    std::string buffer;

    do {
        std::getline(*file, buffer);
    } while (buffer.find("END OF HEADER") == std::string::npos);

    while(true){
        IFileData fileDataBlock;
        std::getline(*file, buffer);

        if(buffer.find("START OF TEC MAP") == std::string::npos)
            break;

        std::getline(*file, buffer);

        int year, month, day, hrs, min, sec;
        std::istringstream(buffer)>> year >> month >> day >> hrs >> min >> sec;

        DateTime moment(2000+year, month, day, hrs, min, sec);

        std::array<std::array<double, 73>, 71> values;

        std::array<std::string, 6> paramsBlock;
        for(auto &block: values){
            for(auto &line: paramsBlock)
                std::getline(*file, line);
            block = parseParamsBlockI(paramsBlock);
        }

        fileDataBlock.h = std::stod(paramsBlock[0].substr(27, 6));
        fileDataBlock.values = values;
        fileDataBlock.moment = moment;

        int i = 0;
        for(auto &lat: fileDataBlock.lat)
            lat = 87.5 - (i++) * 2.5;

        i = 0;
        for(auto &lon: fileDataBlock.lon)
            lon = -180 + (i++) * 5.0;

        fileData.push_back(fileDataBlock);

        std::getline(*file, buffer);
    }
    return fileData;
}
