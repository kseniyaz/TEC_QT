#include "models.h"


double f1(SatParams sat) {
    return(sat.K == 9999) ? GF1 : RF1 + sat.K * DRF1;
}

double f2(SatParams sat) { return(sat.K == 9999) ? GF2 : RF2 + sat.K * DRF2; }


std::vector<double> model(double t, std::vector<double> q) {
    const double r = std::sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2]);
    std::vector<double> dq{
        q[3],
        q[4],
        q[5],
        -MU * q[0] / (r * r * r) + q[0] * UGLE * UGLE + 2 * q[4] * UGLE - 1.5 * J20 * MU * Ra * Ra * (1 - 5 * std::pow(q[2] / r, 2)) / std::pow(r, 5) * q[0],
        -MU * q[1] / (r * r * r) + q[1] * UGLE * UGLE - 2 * q[3] * UGLE - 1.5 * J20 * MU * Ra * Ra * (1 - 5 * std::pow(q[2] / r, 2)) / std::pow(r, 5) * q[1],
        -MU * q[2] / (r * r * r) - 1.5*J20 * MU * Ra * Ra * (3 - 5 * std::pow(q[2] / r, 2)) / std::pow(r, 5) * q[2]
    };
    return  dq;
}

SatParams RSatPosition(GFileParams params, DateTime t1, DateTime t2) {
    std::vector<double> q{ params.X*1000,params.Y*1000,params.Z*1000,params.Vx*1000,params.Vy*1000, params.Vz*1000 };

    double t1UTC = t1.getUTCTime() + params.leap;
    double t2UTC= t2.getUTCTime();

    q = RK4(model, q, t1UTC, t2UTC, 10);

    double dT = 0;
    dT += params.TauN;
    dT += params.TauC;
    dT += params.GammaN * (t2UTC - t1UTC);

    return (SatParams){ .X = q[0],.Y = q[1], .Z = q[2], .dT = dT, .K = params.K };
}

std::vector<RSatsParams> RSatPositions(std::vector<OFileData> &OFileData, std::vector<GFileData> &gFileData){
    std::vector<RSatsParams> satParamsData;

    for(auto &oFileDatum :OFileData){
        RSatsParams satParamsDatum;

        NFileData _gFileMoment;
        auto moment = oFileDatum.moment;
        _gFileMoment.moment = moment;

        std::size_t i = 0;
        for(auto &Rsat: oFileDatum.sats.R){
            if(Rsat.isVisible){
                auto low = std::upper_bound(gFileData.begin(), gFileData.end(), _gFileMoment, [](auto a, auto b){
                    return a.moment < b.moment;
                });

                auto _gFileData = std::vector<GFileData>(gFileData.begin(), low);

                int j = _gFileData.size()-1;

                for(;j>=0;j--){
                    if(_gFileData[j].sats[i].isVisible) break;
                }

                if(j>=0){
                     satParamsDatum.sats[i] = RSatPosition(_gFileData[j].sats[i], _gFileData[j].moment, moment);
                     if((!std::isnan(Rsat.p1)) && (!std::isnan(Rsat.p2))){
                         satParamsDatum.sats[i].P1 = Rsat.p1;
                         satParamsDatum.sats[i].P2 = Rsat.p2;
                     }else{
                         satParamsDatum.sats[i].P1 =  satParamsDatum.sats[i].P2 = std::nan("empty value");
                     }

                    if((!std::isnan(Rsat.c1)) && (!std::isnan(Rsat.c2))){
                         satParamsDatum.sats[i].C1 = Rsat.c1;
                         satParamsDatum.sats[i].C2 = Rsat.c2;
                    }else{
                        satParamsDatum.sats[i].C1 =  satParamsDatum.sats[i].C2 = std::nan("empty value");
                    }
                    if((!std::isnan(Rsat.l1)) && (!std::isnan(Rsat.l2))){
                        satParamsDatum.sats[i].L1 = Rsat.l1;
                        satParamsDatum.sats[i].L2 = Rsat.l2;
                    }else{
                        satParamsDatum.sats[i].L1 =  satParamsDatum.sats[i].L2 = std::nan("empty value");
                    }

                     satParamsDatum.sats[i].isVisible = true;
                }
            }
            i++;
        }
        satParamsDatum.moment = moment;
        satParamsData.push_back(satParamsDatum);
    }
    return satParamsData;
}

SatParams GSatPosition(NFileParams &params, DateTime &t1, DateTime &t2) {
    double n0 = std::sqrt(MU) / std::pow(params.A, 3);

    auto gpst2 = t2.getGPSTime();
    double Tem = gpst2[0];

    double t = Tem - params.Toe;
    double nn = n0 + params.delta_n;
    double MS = params.M0 + nn * t;
    double E = MS;
    double rez = E + 1;
    while (std::abs(rez - E) > 1e-10) {
        rez = E;
        E = MS + params.e * std::sin(E);
    }
    double v = std::atan2(std::sqrt(1 - params.e * params.e) * std::sin(E), std::cos(E) - params.e);
    double f = v + params.w0;
    double delu = params.Cus * std::sin(2 * f) + params.Cuc * std::cos(2 * f);
    double delr = params.Crs * std::sin(2 * f) + params.Crc * std::cos(2 * f);
    double deli = params.Cis * std::sin(2 * f) + params.Cic * std::cos(2 * f);
    double u = f + delu;
    double r = params.A * params.A * (1 - params.e * std::cos(E)) + delr;
    double ii = params.i0 + params.IDOT * t + deli;
    double ugl = params.Omega0 + t * (params.Omega_p - UGLE) - UGLE * params.Toe;

    double Xorb = r * std::cos(u);
    double Yorb = r * std::sin(u);

    double x = Xorb * std::cos(ugl) - Yorb * std::cos(ii) * std::sin(ugl);
    double y = Xorb * std::sin(ugl) + Yorb * std::cos(ii) * std::cos(ugl);
    double z = Yorb * std::sin(ii);

    auto gpst1 = t1.getGPSTime();
    double Toc = gpst1[0];

    double dT =
        -2. * std::sqrt(MU) * params.A * params.e * std::sin(E) / std::sqrt(SPEED_OF_LIGHT)
        + params.af0
        + params.af1 * (Tem - Toc)
        + params.af2 * (Tem - Toc) * (Tem - Toc);
    return (SatParams) { .X = x, .Y = y, .Z = z, .dT = dT, .K = 9999 };
}

std::vector<GSatsParams> GSatPositions(std::vector<OFileData> &OFileData, std::vector<NFileData> &nFileData){
    std::vector<GSatsParams> satParamsData;

    for(auto &oFileDatum :OFileData){
        GSatsParams satParamsDatum;

        NFileData _gFileMoment;
        auto moment = oFileDatum.moment;
        _gFileMoment.moment = moment;

        std::size_t i = 0;
        for(auto &Gsat: oFileDatum.sats.G){
            if(Gsat.isVisible){
                auto low = std::upper_bound(nFileData.begin(), nFileData.end(), _gFileMoment, [](auto a, auto b){
                    return a.moment < b.moment;
                });

                auto _nFileData = std::vector<NFileData>(nFileData.begin(), low);

                int j = _nFileData.size()-1;

                for(;j>=0;j--){
                    if(_nFileData[j].sats[i].isVisible) break;
                }

                if(j>=0){
                     satParamsDatum.sats[i] = GSatPosition(_nFileData[j].sats[i], _nFileData[j].moment, moment);
                     if((!std::isnan(Gsat.p1)) && (!std::isnan(Gsat.p2))){
                         satParamsDatum.sats[i].P1 = Gsat.p1;
                         satParamsDatum.sats[i].P2 = Gsat.p2;
                     }else{
                         satParamsDatum.sats[i].P1 = satParamsDatum.sats[i].P2 = std::nan("empty value");
                     }

                    if((!std::isnan(Gsat.c1)) && (!std::isnan(Gsat.c2))){
                         satParamsDatum.sats[i].C1 = Gsat.c1;
                         satParamsDatum.sats[i].C2 = Gsat.c2;
                    }else{
                        satParamsDatum.sats[i].C1 = satParamsDatum.sats[i].C2 = std::nan("empty value");
                    }
                    if((!std::isnan(Gsat.l1)) && (!std::isnan(Gsat.l2))){
                        satParamsDatum.sats[i].L1 = Gsat.l1;
                        satParamsDatum.sats[i].L2 = Gsat.l2;
                    }else{
                        satParamsDatum.sats[i].L1 = satParamsDatum.sats[i].L2 = std::nan("empty value");
                    }
                    satParamsDatum.sats[i].isVisible = true;
                }
            }
            i++;
        }
        satParamsDatum.moment = moment;
        satParamsData.push_back(satParamsDatum);
    }
    return satParamsData;
}
