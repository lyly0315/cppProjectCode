#include "CurveDiscount.h"
#include "Market.h"
#include "Streamer.h"

#include <cmath>


namespace minirisk {

CurveDiscount::CurveDiscount(Market *mkt, const Date& today, const string& curve_name)
    : m_today(today)
    , m_name(curve_name)
    , m_rate(mkt->get_yield(curve_name.substr(ir_curve_discount_prefix.length(),3)))
{
    //std::cout << "m_name#####:" << m_name << std::endl;
    local_rates(mkt);

}

struct RatePairDoubleComparator {
    bool operator() (
        const double& left, const std::pair<double, double>& right) {
        return left < right.first;
    }
};

double CurveDiscount::df(const Date& t) const
{
    MYASSERT((!(t < m_today)), "cannot get discount factor for date in the past: " << t);
    double dt = time_frac(m_today, t);
    
    // Use yield.
    if (log_dfs.size() == 1) {
        return std::exp(-m_rate * dt);
    }
    auto it = std::upper_bound(
        log_dfs.begin(), log_dfs.end(), dt, RatePairDoubleComparator());
    if (it == log_dfs.end()) {
        if (dt == log_dfs.back().first) {
            return std::exp(log_dfs.back().second);
        }
        MYASSERT(false,
            "Curve " << m_name << ", DF not available beyond last tenor date "
            << dwmy_date << ", requested " << t);
    }
    else {
        const auto& t2_log_df = *it;
        const auto& t1_log_df = *(--it);
        return std::exp(
            ((t2_log_df.first - dt) * t1_log_df.second
                + (dt - t1_log_df.first) * t2_log_df.second)
            / (t2_log_df.first - t1_log_df.first));
    }

    //return std::exp(-m_rate * dt);
}

int CurveDiscount::DWMY_days(std::string dwmy) {
    int len = dwmy.length();
    int times = std::stoi(dwmy.substr(0, len - 1));
    char date = dwmy.c_str()[len - 1];
    int days = 0;
    switch (date) {
    case 'D':
        days = 1;
        break;
    case 'W':
        days = 7;
        break;
    case 'M':
        days = 30;
        break;
    case 'y':
        days = 365;
        break;
    }
    int days_total = times * days;
    if (days_total != 0) {
        return days_total;
    }
    else {
        std::cout << "Error days" << std::endl;
        return 0;
    }
}

void CurveDiscount::local_rates(Market* mkt) {
    std::string currency = m_name.substr(m_name.length() - 3);
    std::string regex = ir_rate_prefix + "[0-9]+(D|W|M|Y)\\." + currency;//ir_rate_prefix is "IR."
    const auto& matched = mkt->fetch_risk_factors(regex);// matched is IR + its rate
    log_dfs.push_back(std::make_pair(0.0, 0.0));
    //std::cout << "log_dfs########:" << ir_rate_prefix << std::endl;
    std::vector<std::pair<int, double>> dwmy_rates;
    for (const auto& rate : matched) {
        std::string tenor = rate.first.substr(
            ir_rate_prefix.length(),
            rate.first.length() - 4 - ir_rate_prefix.length());
        dwmy_rates.push_back(
            std::make_pair(DWMY_days(tenor), rate.second));
    }
    std::sort(dwmy_rates.begin(), dwmy_rates.end());
    for (const auto& rate : dwmy_rates) {
        double tf = rate.first / 365.0;
        double df = -rate.second * tf;
        log_dfs.push_back(std::make_pair(tf, df));
    }

    // Init from yield.
    if (dwmy_rates.empty()) {
        m_rate = mkt->get_yield(currency);
        //std::cout << "m_rate######:" << m_rate << std::endl;
    }
    else {
        dwmy_date = m_today + dwmy_rates.back().first;
        //std::cout << "m_today######:" << dwmy_date << std::endl;
    }
}


} // namespace minirisk
