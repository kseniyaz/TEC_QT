#include "dateTime.h"

DateTime::DateTime(int year, int month, int day, int hour ,int minute, double second):
year(year),month(month),day(day),hour(hour),minute(minute),second(second), isDateTime(true){
    boost::gregorian::date date(year, month, day);
    boost::posix_time::time_duration  time(hour, minute, second);
    ptime = boost::posix_time::ptime(date, time);
}

DateTime::DateTime(){}

bool DateTime::is_not_a_date_time(){return !isDateTime; }

bool DateTime::operator==( const DateTime& rhs){return ptime == rhs.ptime; }

bool DateTime::operator>=( const DateTime& rhs){ return ptime >= rhs.ptime; }

bool DateTime::operator<=( const DateTime& rhs){ return ptime <= rhs.ptime; }

bool DateTime::operator>( const DateTime& rhs){ return ptime > rhs.ptime; }

bool DateTime::operator<( const DateTime& rhs){ return ptime < rhs.ptime; }

bool DateTime::operator!=( const DateTime& rhs){ return ptime != rhs.ptime; }

boost::posix_time::ptime DateTime::get_posix_time(){ return ptime; };

double DateTime::getUTCTime(){
    const int doy[] = { 1,32,60,91,121,152,182,213,244,274,305,335 };

    int days = (year - 1970) * 365 + (year - 1969) / 4 + doy[month - 1] + day - 2 + (year % 4 == 0 && month >= 3 ? 1 : 0);
    return days * 86400 + hour * 3600 + minute * 60 + second;
}

std::array<double, 2> DateTime::getGPSTime(){
    double t = this->getUTCTime();
    double t0 =  DateTime( 1980, 1, 6, 0, 0, 0 ).getUTCTime();
    double sec = t - t0;
    double week = (int)(sec / (86400 * 7));

    std::array<double, 2> GPSTime = {sec - week * 86400 * 7, week};
    return GPSTime;
}
