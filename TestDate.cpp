#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "Date.h"
#include "Macros.h"


//generate 1000 invalid dates and test whether structor Date can check them successfully
void test1()
{
    unsigned invalid_number = 0;
    unsigned error_number = 0;
    std::ofstream f("TestDate_test1.txt");
    while (invalid_number < 1000) {
        unsigned y, m, d;
        y = std::rand() % 500 + 1800;
        m = std::rand() % 12 + 1;
        d = std::rand() % 32 + 1;
        f << y << "/" << m << "/" << d << " ";
        if (!minirisk::Date::valid_test(y, m, d)) 
        {
            invalid_number += 1;
            try 
            {
                minirisk::Date date(y, m, d);
            }
            catch (...) 
            {
                error_number += 1;
            }
        }
    }

    try
    {
        invalid_number == 1000 && error_number == 1000;
    }
    catch (...)
    {
        std::cout << "test1 fail" << std::endl;
    }
}


//Verify that converting a date in calendar format (day, month, year) to serial format

void test2()
{
    unsigned error_number = 0;
    std::ofstream f("TestDate_test2.txt");
    for (unsigned y1 = minirisk::Date::first_year; y1 < minirisk::Date::last_year; ++y1)
        for (unsigned m1=1;m1<=12;++m1)
            for (unsigned d1 = 1; d1 <= (minirisk::Date::is_leap_year(y1) ? minirisk::Date::leap_days_in_month[m1-1] : minirisk::Date::days_in_month[m1-1]);++d1)
            {
                unsigned y2, m2, d2;
                minirisk::Date date(y1,m1,d1);
                date.year_month_day(date, &y2, &m2, &d2);
                f << d1 << "-" << m1 << "-" << y1 << " ";
                f << d2 << "-" << m2 << "-" << y2 << "/n";
                if (y1 != y2 || m1 != m2 || d1 != d2)
                {
                    error_number += 1;
                }
                    

            }
    
    try
    {
        error_number == 0;
    }
    catch (...)
    {
        std::cout << "test2 fail" << std::endl;
    }
}

void test3()
{
    std::ofstream f("TestDate_test3.txt");
    for (unsigned y1 = minirisk::Date::first_year; y1 < minirisk::Date::last_year-1; ++y1)
        for (unsigned m1 = 1; m1<=12; ++m1)
        {
            unsigned y2, m2, d2;
            unsigned d1 = (minirisk::Date::is_leap_year(y1) ? minirisk::Date::leap_days_in_month[m1 - 1] : minirisk::Date::days_in_month[m1 - 1]);
            minirisk::Date date1(y1, m1, d1);
            if (m1 == 12)
            {
                y2 = y1 + 1;
                m2 = 1;
                d2 = 1;
            }
            else
            {
                y2 = y1;
                m2 = m1 + 1;
                d2 = 1;
            }
            minirisk::Date date2(y2, m2, d2);
            f << y1 << "/" << m1 << "/" << d1 << " ";
            f << y2 << "/" << m2 << "/" << d2 << "/n";
            try
            {
                date2 - date1 == 1;
            }
            catch (...)
            {
                std::cout << "test3 fail" << std::endl;
            }
        }

    unsigned y1 = minirisk::Date::last_year - 1;
    for (unsigned m1 = 1; m1 <12; ++m1)
    {
        unsigned d2 = 1;
        unsigned d1 = (minirisk::Date::is_leap_year(y1) ? minirisk::Date::leap_days_in_month[m1 - 1] : minirisk::Date::days_in_month[m1 - 1]);
        minirisk::Date date1(y1, m1, d1);
        minirisk::Date date2(y1, m1+1, d2);
        f << d1 << "-" << m1 << "-" << y1 << " ";
        f << d2 << "-" << m1 + 1 << "-" << y1 << "/n";
        try 
        {
            date2 - date1 == 1;
        }
        catch (...) 
        {
            std::cout << "test3 fail" << std::endl;
        }
    }

}

int main()
{
    test1();
    test2();
    test3();
    std::cout << "SUCCESS" << std::endl;
    return 0;
}

