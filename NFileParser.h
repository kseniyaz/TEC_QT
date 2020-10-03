#ifndef NFILEPARSER_H
#define NFILEPARSER_H

#include <fstream>
#include <vector>
#include <array>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "dateTime.h"

struct NFileParams{
    double af0, af1, af2;

    double Crs;
    double delta_n;
    double M0;

    double Cuc;
    double e;
    double Cus;
    double A;

    double Toe;
    double Cic;
    double Omega0;
    double Cis;

    double i0;
    double Crc;
    double w0;
    double Omega_p;

    double IDOT;

    double Tgd;

    bool isVisible = false;
};

struct NFileData{
    DateTime moment;
    std::array<NFileParams, 32> sats;
};

std::vector<NFileData> parseNFile(std::ifstream*);

#endif // NFILEPARSER_H
