#include "OFileParser.h"

inline std::vector<SatName> parseNamesLine(std::string& line){
    std::vector<SatName> namesArray(12);
    std::size_t i=0;
    for(auto &name: namesArray){
        try {
            name.type = line[32 + i * 3];
            name.number = std::stoi(line.substr(33 + i * 3, 3));

            i++;
        }
        catch (std::exception&) {
            break;
        }

    }
    return namesArray;
};

inline void parseLine(std::array<double*, 5>& fields, std::string& line){
    int i=0;
    for(auto field: fields){
        if(field)
             try {
                 *field = std::stod(line.substr(16 * i, 14));

             } catch (std::exception&) {
                 *field = std::nan("empty value");
             }
        i++;
    }
};

inline OFileParams parseParamsSubBlock(std::array<std::string, 2> subBlock){
    OFileParams params;
    params.isVisible = true;

    std::array<std::array<double*, 5>, 2> lines={
        std::array<double*, 5>{&params.c1, &params.p1, &params.l1, &params.c2, &params.p2},
        std::array<double*, 5>{ &params.l2, nullptr, nullptr, nullptr, nullptr},
    };

    int i=0;
    for(auto &line: subBlock)
        parseLine(lines[i++], line);
    return params;
}

std::vector<OFileData> parseOFile(std::ifstream* file){
    std::vector<OFileData> fileData;
    std::string buffer;

    do {
        std::getline(*file, buffer);
    } while (buffer.find("END OF HEADER") == std::string::npos);

    while(true){
        std::getline(*file, buffer);

        if(file->eof())
            break;

        int year, month, day, hrs, min, sec;
        std::istringstream(buffer)>> year >> month >> day >> hrs >> min >> sec;

        if (sec != 0 && sec!= 30) {
            for(int i=0; i<7; i++)
                std::getline(*file, buffer);
            continue;
        }

        DateTime moment(2000+year, month, day, hrs, min, sec);

        std::size_t numberOfSatS = std::stoi(buffer.substr(30, 3));

        std::vector<SatName> namesArray;

        {
            std::size_t numberOfNamesLine = std::ceil(numberOfSatS / 12.);
            for(std::size_t i = 0; i < numberOfNamesLine; i++){
                auto namesLineArray = parseNamesLine(buffer);
                namesArray.insert(namesArray.end(), namesLineArray.begin(),  namesLineArray.end());
                if(i < numberOfNamesLine - 1)
                    std::getline(*file, buffer);
            }
        }

        std::vector<std::array<std::string, 2>> paramsBlock(numberOfSatS);

        for(auto &subBlock: paramsBlock)
            for(auto &line: subBlock)
                std::getline(*file, line);

        OFileData fileDataBlock;
        std::size_t i = 0;
        for(auto &subBlock: paramsBlock){
            auto params = parseParamsSubBlock(subBlock);
            switch (namesArray[i].type) {
                case 'G': fileDataBlock.sats.G[namesArray[i].number - 1] = params; break;
                case 'R': fileDataBlock.sats.R[namesArray[i].number - 1] = params; break;
            }
            i++;
        }
        fileDataBlock.moment = moment;
        fileData.push_back(fileDataBlock);
    }
    return fileData;
}
