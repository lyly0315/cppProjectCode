#include <iomanip>
#include <algorithm>
#include <iostream>
#include "Date.h"
#include <xtree>

namespace minirisk {

struct DateInitializer : std::array<unsigned, Date::n_years>
{
    DateInitializer()
    {
        for (unsigned i = 0, s = 0, y = Date::first_year; i < size(); ++i, ++y) {
            (*this)[i] = s;
            s += 365 + (Date::is_leap_year(y) ? 1 : 0);
        }
    }
};

const std::array<unsigned, 12> Date::days_in_month = { {31,28,31,30,31,30,31,31,30,31,30,31} };
//*闰年每月天数
const std::array<unsigned, 12> Date::leap_days_in_month = { {31,29,31,30,31,30,31,31,30,31,30,31} };
const std::array<unsigned, 12> Date::days_ytd{ {0,31,59,90,120,151,181,212,243,273,304,334} };
//*闰年年天数
const std::array<unsigned, 12> Date::leap_days_ytd{ {0,31,60,91,121,152,182,213,244,274,305,335} };
const std::array<unsigned, Date::n_years> Date::days_epoch(static_cast<const std::array<unsigned, Date::n_years>&>(DateInitializer()));


/* The function checks if a given year is a leap year.
    Leap year must be a multiple of 4, but it cannot be a multiple of 100 without also being a multiple of 400.
*/
bool Date::is_leap_year(unsigned year)
{
    return ((year % 4 != 0) ? false : (year % 100 != 0) ? true : (year % 400 != 0) ? false : true);
}


// The function pads a zero before the month or day if it has only one digit.
std::string Date::padding_dates(unsigned month_or_day)
{
    std::ostringstream os;
    os << std::setw(2) << std::setfill('0') << month_or_day;
    return os.str();
}

//*判断日期是否有效，年月日是否在区间
bool Date::valid_test(unsigned y, unsigned m, unsigned d)
{
    if ((y >= first_year) && (y < last_year) 
        && (m >= 1) && (m <= 12) 
        && (d >= 1) && (d <= (is_leap_year(y)? leap_days_in_month[m-1]: days_in_month[m-1])))
        return true;
    else
        return false;
}
//*若日期无效，返回invalid date
void Date::check_valid(unsigned y, unsigned m, unsigned d)
{
    MYASSERT(valid_test(y, m, d), "invalid date: " << d << "-" << m << "-" << y);
}



//*将给定的serial转换为年月日
void Date::year_month_day(Date date, unsigned* y, unsigned* m, unsigned* d)
{
    //*求年份
    auto y_i = std::upper_bound(days_epoch.begin(),days_epoch.end(),date.getserial());
    --y_i;
    *y = y_i-days_epoch.begin() + Date::first_year;
    //*求月份
    auto m_i = (is_leap_year(*y)) ? 
        std::upper_bound(leap_days_ytd.begin(),leap_days_ytd.end(), date.getserial()-*y_i)
        : std::upper_bound(days_ytd.begin(), days_ytd.end(), date.getserial() - *y_i);
    --m_i;
    *m = m_i +1 - (is_leap_year(*y) ? leap_days_ytd.begin() : days_ytd.begin());
    //求日期
    *d=  date.getserial()+1-* y_i- * m_i;
}


/*  The function calculates the distance between two Dates.
    d1 > d2 is allowed, which returns the negative of d2-d1.
*/
long operator-(const Date& d1, const Date& d2)
{
    unsigned s1 = d1.getserial();
    unsigned s2 = d2.getserial();
    return static_cast<long>(s1) - static_cast<long>(s2);
}

} // namespace minirisk

