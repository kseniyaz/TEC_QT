#include "NFileParser.h"

inline void parseParamsLineN(std::array<double*, 4>& fields, std::string& line){
    int i=0;
    for(auto field: fields){
        if(field)
            *field = std::stod(line.substr(3+19*i, 20));
        i++;
    }
};

inline NFileParams parseParamsBlockN(std::vector<std::string>& block){
    NFileParams params;
    params.isVisible = true;

    std::array<std::array<double*, 4>, 8> lines={
        std::array<double*, 4>{nullptr, &params.af0, &params.af1, &params.af2},
        std::array<double*, 4>{nullptr, &params.Crs, &params.delta_n, &params.M0},
        std::array<double*, 4>{&params.Cuc, &params.e, &params.Cus, &params.A},
        std::array<double*, 4>{&params.Toe, &params.Cic, &params.Omega0, &params.Cis},
        std::array<double*, 4>{&params.i0, &params.Crc, &params.w0, &params.Omega_p},
        std::array<double*, 4>{&params.IDOT, nullptr, nullptr, nullptr},
        std::array<double*, 4>{nullptr, nullptr, &params.Tgd, nullptr},
        std::array<double*, 4>{nullptr, nullptr, nullptr, nullptr}
    };
    int i=0;
    for(auto &line: block){
        boost::replace_all(line, "D", "e");
        parseParamsLineN(lines[i++], line);
    }
    return params;
}

std::vector<NFileData> parseNFile(std::ifstream* file){
    std::vector<NFileData> fileData;
    std::string buffer;

    do {
        std::getline(*file, buffer);
    } while (buffer.find("END OF HEADER") == std::string::npos);

    DateTime previousMoment;
    NFileData fileDataBlock;

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

        DateTime moment(2000 + year, month, day, hrs, min, sec);

        std::vector<std::string> paramsBlock(7);

        for(auto &line: paramsBlock)
            std::getline(*file, line);

        paramsBlock.insert(paramsBlock.begin(), buffer);

        auto params = parseParamsBlockN(paramsBlock);

        if(!previousMoment.is_not_a_date_time() && (previousMoment<moment)){
            fileData.push_back(fileDataBlock);
            fileDataBlock = NFileData();
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
