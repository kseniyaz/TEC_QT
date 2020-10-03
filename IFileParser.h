#ifndef IFILEPARSER_H
#define IFILEPARSER_H

#include <fstream>
#include <vector>
#include <array>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "dateTime.h"

struct IFileData{
    std::array<double, 71> lat;
    std::array<double, 73> lon;
    std::array<std::array<double, 73>, 71> values;
    double h;
    DateTime moment;
};

std::vector<IFileData> parseIFile(std::ifstream*);

#endif // IFILEPARSER_H
