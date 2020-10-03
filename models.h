#ifndef MODELS_H
#define MODELS_H

#include <vector>
#include <math.h>
#include <algorithm>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

#include "dateTime.h"
#include "utils.h"
#include "GFileParser.h"
#include "NFileParser.h"
#include "OFileParser.h"

const double GF1 = 1575420000.0;
const double GF2 = 1227600000.0;
const double RF1 = 1602000000.0;
const double RF2 = 1246000000.0;
const double DRF1=     562500.0;
const double DRF2=     437500.0;
const double UGLE = 0.000072921151467;
const double MU   = 398600441800000.0;
const double J20  = 1082.62982126e-6;
const double Ra   = 6378137.0;
const double SPEED_OF_LIGHT = 299792458.0;

struct SatParams{
    double X;
    double Y;
    double Z;
    double dT;
    double P;
    double P1;
    double P2;
    double L1;
    double L2;
    double C1;
    double C2;
    double C;
    double K;
    bool isVisible = false;
};

struct RSatsParams{
    DateTime moment;
    std::array<SatParams, 32> sats;
};

struct GSatsParams{
    DateTime moment;
    std::array<SatParams, 24> sats;
};


double f1(SatParams sat);

double f2(SatParams sat);

std::vector<double> model(double, std::vector<double>);
std::vector<RSatsParams> RSatPositions(std::vector<OFileData>& OFileData, std::vector<GFileData>& gFileData);
std::vector<GSatsParams> GSatPositions(std::vector<OFileData>&, std::vector<NFileData>&);

#endif // MODELS_H
