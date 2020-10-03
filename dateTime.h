#ifndef DATETIME_H
#define DATETIME_H

#include <array>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>

class DateTime {
public:

    DateTime(int, int, int, int ,int, double);
    DateTime();

    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;

    double getUTCTime();
    std::array<double, 2> getGPSTime();

    bool operator==(const DateTime&);
    bool operator>=(const DateTime&);
    bool operator<=(const DateTime&);
    bool operator>(const DateTime&);
    bool operator<(const DateTime&);
    bool operator!=(const DateTime&);
    bool is_not_a_date_time();
    boost::posix_time::ptime get_posix_time();
private:
    boost::posix_time::ptime ptime;
    bool isDateTime = false;
};



#endif // DATETIME_H
