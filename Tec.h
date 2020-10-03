
#ifndef TEC_H
#define TEC_H

#include <array>
#include <cmath>
#include <vector>
#define _USE_MATH_DEFINES

#include "models.h"
#include "utils.h"
#include "dateTime.h"

const double H_ION = 450000.0;
const double RC = 6371000.0;
const double K_ION = 40.308;

struct TECval{
    double tecC = nan("empty value");
    double tecL = nan("empty value");
};

struct TECvalR{
    std::array<TECval, 24> sat;
    DateTime moment;
};

struct TECvalG{
    std::array<TECval, 32> sat;
    DateTime moment;
};


template < class TEC_VAL, class SAT_PARAMS>
std::vector<TEC_VAL> TEC(std::vector<SAT_PARAMS>& satParamsData,std::array<double, 3>& pointPos) {
    std::vector<TEC_VAL> TECData;
    double Rr = l2<double, 3>(pointPos);

    for(auto satParams: satParamsData){
        TEC_VAL TECDatum;
        std::size_t i =0;
        for(auto &sat: satParams.sats){
            if(sat.isVisible){
                std::array<double, 3> RsRr{sat.X -  pointPos[0], sat.Y -  pointPos[1], sat.Z -  pointPos[2]};
                double cosz = (RsRr[0]*pointPos[0] + RsRr[1] *pointPos[1] + RsRr[2] *pointPos[2]) / (Rr * l2<double, 3>(RsRr));
                if (std::asin(cosz) * 180 / M_PI < 10) continue;
                double cosz_ = pow((1 - (1 - cosz * cosz) * pow(Rr / (RC + H_ION), 2)), 0.5);
                cosz_=1;
                TECval TECValues;

                if(!isnan(sat.P2) && !isnan(sat.P1))
                     TECValues.tecC = (sat.P2                                   - sat.P1                                  ) * std::pow(f1(sat) * f2(sat), 2) / (K_ION * (std::pow(f1(sat), 2) - std::pow(f2(sat), 2)))* cosz_ / pow(10, 16);

                if(!isnan(sat.L1) && !isnan(sat.L2)){
                    TECValues.tecL = (sat.L1*SPEED_OF_LIGHT / f1(sat) - sat.L2*SPEED_OF_LIGHT / f2(sat)) * std::pow(f1(sat) * f2(sat), 2) / (K_ION * (std::pow(f1(sat), 2) - std::pow(f2(sat), 2)))* cosz_ / pow(10, 16);
                }
                TECDatum.sat[i] = TECValues;
            }
            i++;
        }
        TECDatum.moment = satParams.moment;
        TECData.push_back(TECDatum);
    }
    return TECData;
}

#endif

