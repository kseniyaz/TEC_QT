#ifndef GFILEPARSER_H
#define GFILEPARSER_H

#include <fstream>
#include <vector>
#include <array>
#include <string>

#include <boost/algorithm/string/replace.hpp>

#include "dateTime.h"

struct GFileParams{
    double X;
    double Y;
    double Z;
    double Vx;
    double Vy;
    double Vz;

    double TauN;
    double GammaN;
    double leap;
    double K;
    double TauC;
    bool isVisible = false;
};

struct GFileData{
    DateTime moment;
    std::array<GFileParams, 24> sats;
};

std::vector<GFileData> parseGFile(std::ifstream*);

#endif // GFILEPARSER_H
