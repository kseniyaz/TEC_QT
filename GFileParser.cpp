#include "GFileParser.h"

void parseParamsLineG(std::array<double*, 4>& fields, std::string& line){
    int i=0;
    for(auto field: fields){
        if(field)
            *field = std::stod(line.substr(3+19*i, 20));
        i++;
    }
};

GFileParams parseParamsBlockG(std::vector<std::string>& block){
    GFileParams params;
    params.isVisible = true;

    std::array<std::array<double*, 4>, 8> lines={
        std::array<double*, 4>{nullptr, &params.TauN, &params.GammaN, nullptr},
        std::array<double*, 4>{&params.X, &params.Vx, nullptr, nullptr},
        std::array<double*, 4>{&params.Y, &params.Vy, nullptr, &params.K},
        std::array<double*, 4>{&params.Z, &params.Vz, nullptr, nullptr},
    };
    std::size_t i=0;
    for(auto &line: block){
        boost::replace_all(line, "D", "e");
        parseParamsLineG(lines[i++], line);
    }

    return params;
}

std::vector<GFileData> parseGFile(std::ifstream* file){
    std::vector<GFileData> fileData;
    std::string buffer;

    double TauC, leap;

    do {
        std::getline(*file, buffer);
        if(buffer.find("CORR TO SYSTEM TIME") != std::string::npos){
             boost::replace_all(buffer, "D", "e");
             leap = std::stod(buffer.substr(21, 20));
        }
        if(buffer.find("LEAP SECONDS") != std::string::npos)
             TauC = std::stod(buffer.substr(4, 3));
    } while (buffer.find("END OF HEADER") == std::string::npos);

    DateTime previousMoment;
    GFileData fileDataBlock;

    while(true){
        std::getline(*file, buffer);

        if(file->eof())
            break;

        int year, month, day, hrs, min, sec, satNumber;
        std::istringstream(buffer)>> satNumber >> year >> month >> day >> hrs >> min >> sec;

        if (sec != 0 && sec!= 30) {
            for(int i=0; i<7; i++)
                std::getline(*file, buffer);
            continue;
        }

        DateTime moment(2000+year, month, day, hrs, min, sec);
        std::vector<std::string> paramsBlock(3);

        for(auto &line: paramsBlock)
            std::getline(*file, line);

        paramsBlock.insert(paramsBlock.begin(), buffer);

        auto params = parseParamsBlockG(paramsBlock);
        params.TauC = TauC;
        params.leap = leap;

        if(!previousMoment.is_not_a_date_time() && (previousMoment<moment)){
            fileData.push_back(fileDataBlock);
            fileDataBlock = GFileData();
            fileDataBlock.moment = moment;
            fileDataBlock.sats[satNumber - 1] = params;
        }else{
            fileDataBlock.moment = moment;
            fileDataBlock.sats[satNumber - 1] = params;
        }
        previousMoment = moment;
    }
    return fileData;
}
