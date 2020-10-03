#ifndef UTILS_H
#define UTILS_H

#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

std::vector<double> RK4(std::vector<double>(*der)(double, std::vector<double>), std::vector<double>&, double& t1, double& t2,const double& h);

template<class T, int N>
T l2(std::array<T, N> arr){
    double sum = 0;
    for(auto val: arr)
        sum +=(val*val);
    return std::sqrt(sum);
}

#endif // UTILS_H
