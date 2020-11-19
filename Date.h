#pragma once

#include "Macros.h"
#include <string>
#include <array>

namespace minirisk {

struct Date
{
public:
    static const unsigned first_year = 1900;
    static const unsigned last_year = 2200;
    static const unsigned n_years = last_year - first_year;

private:
    static std::string padding_dates(unsigned);

    // number of days elapsed from beginning of the year
    unsigned day_of_year() const;

    friend long operator-(const Date& d1, const Date& d2);



public:
    static const std::array<unsigned, 12> days_in_month;  // num of days in month M in a normal year
    static const std::array<unsigned, 12> leap_days_in_month;  // num of days in month M in a leap year
    static const std::array<unsigned, 12> days_ytd;      // num of days since 1-jan to 1-M in a normal year
    static const std::array<unsigned, 12> leap_days_ytd;      // num of days since 1-jan to 1-M in a leap year
    static const std::array<unsigned, n_years> days_epoch;   // num of days since 1-jan-1900 to 1-jan-yyyy (until 2200)

    // Default constructor
    Date() : m_serial(0) {}
    //*input serial
    Date(unsigned serial) : m_serial(serial) {}
   
    // Constructor where the input value is checked.
    //*给定天数时自动赋值m_serial
    //*给定字符串时赋值m_serial
    //stoul将string转为unsigned
    Date(const std::string& yyyymmdd)
    {
        m_serial = stoul(yyyymmdd);
    }
    //*给定日期时需要进行转换为天数（通过init函数）
    Date(unsigned year, unsigned month, unsigned day)
    {
        init(year, month, day);
    }

    //*若给定yearmonthday，init将转换为m_serial(即给定日期距离初始日期的天数)
    void init(unsigned year, unsigned month, unsigned day)
    {
        check_valid(year, month, day);
        unsigned month_days;
        month_days = (is_leap_year(year)) ? leap_days_ytd[month -1] : days_ytd[month - 1];
        //*serial=num days of former years+num days of former months in current year+num of days in current month
        m_serial = days_epoch[year - first_year] + month_days + day - 1;
    }

    //*若给定天数，init将转换为m_serial
    void init(unsigned serial)
    {
        //if (valid_test(serial))
            m_serial = serial;
    }


    /*Date(const std::string& yyyymmdd) :
        Date(std::stoul(yyyymmdd.substr(0, 4))，std::stoul(yyyymmdd.substr(4, 2))，std::stoul(yyyymmdd.substr(6)))
    {}*/
    //*判断日期是否有效
    static void check_valid(unsigned y, unsigned m, unsigned d);
    static bool valid_test(unsigned y, unsigned m, unsigned d);
    //*判断是否为闰年
    static bool is_leap_year(unsigned year);
    //*将距离起始日期的天数serial转换为年月日的日期
    static void year_month_day(Date date, unsigned* y, unsigned* m, unsigned* d);

    
    //*number of days since 1-Jan-1900
    unsigned getserial() const
    {
        return m_serial;
    }

    //*运算符-大小关系
    bool operator<(const Date& d) const
    {
        return m_serial < d.getserial();
    }

    bool operator==(const Date& d) const
    {
        return m_serial <= d.getserial();
    }

    bool operator>(const Date& d) const
    {
        return m_serial > d.getserial();
    }


    //*运算符-加减
    Date operator+(unsigned n) const
    {
        return Date(getserial() + n);
    }

    Date operator-(unsigned n) const
    {
        return Date(getserial() - n);
    }

    // In YYYYMMDD format
    std::string to_string(bool pretty = true) const
    {
        unsigned m_y=0, m_m=0, m_d=0;
        return pretty
            ? std::to_string((int)m_d) + "-" + std::to_string((int)m_m) + "-" + std::to_string(m_y)
            : std::to_string(m_serial);
    }
    
private:
    //*change yr,mth,day->serial
    unsigned m_serial;
};

long operator-(const Date& d1, const Date& d2);

inline double time_frac(const Date& d1, const Date& d2)
{
    return static_cast<double>(d2 - d1) / 365.0;
}

} // namespace minirisk
