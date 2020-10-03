#ifndef OFILEPARSER_H
#define OFILEPARSER_H

#include <fstream>
#include <vector>
#include <array>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

#include "dateTime.h"

struct OFileParams{
    double p1;
    double p2;
    double l1;
    double l2;
    double c1;
    double c2;
    bool isVisible = false;
};

struct OFileSatTypes{
    std::array<OFileParams, 32> G;
    std::array<OFileParams, 24> R;
} ;

struct OFileData{
    DateTime moment;
    OFileSatTypes sats;
};

std::vector<OFileData> parseOFile(std::ifstream*);

struct SatName{
    int number;
    char type;
};

#endif // OFILEPARSER_H
