#pragma once
#include <vector>
#include "ICurve.h"

//using std::vector;

namespace minirisk {

struct Market;

struct CurveDiscount : ICurveDiscount
{
    virtual string name() const { return m_name; }

    CurveDiscount(Market *mkt, const Date& today, const string& curve_name);

    void local_rates(Market* mkt);

    // compute the discount factor
    double df(const Date& t) const;

    virtual Date today() const { return m_today; }

    int DWMY_days(std::string dwmy);

private:
    
    Date m_today;
    //Date m_last_tenor_date;
    Date dwmy_date;
    string m_name;
    std::vector<std::pair<double, double>> log_dfs;
    //std::vector< std::pair<double, double> > log_dfs;
    double m_rate;
};

} // namespace minirisk
