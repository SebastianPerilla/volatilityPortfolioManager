#pragma once

#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>
#include <map>

namespace VolatilityFunctions {

/**
 * @brief Calculates the average of a vector of doubles.
 * 
 * @param return_list A vector containing the data points.
 * @return The average value of the data points.
 */
// Return datatype: double 
double average(std::vector<double>& return_list) {
    double average = std::accumulate(return_list.begin(), return_list.end(), 0.0) / return_list.size();
    return average;
};

/**
 * @brief Computes the average return for a given time period.
 * 
 * @param r_average_list A vector of returns for the given period.
 * @return The average return.
 */
double average_return(std::vector<double> r_average_list) {
    double r_bar = average(r_average_list);
    return r_bar;
};


/**
 * @brief Computes the logarithmic returns for a given price vector.
 * 
 * @param price A vector of stock prices over time.
 * @return A vector of logarithmic returns.
 */
std::vector<double> logarithmic_return_function(std::vector<double>& price) {
    std::vector<double> r_t_list;
    
    // Using a regular iterator to loop through all the initial prices
    for (std::vector<double>::iterator it = price.begin(); it != price.end()-1; ++it) {
        double r_t = log(*(it + 1) / *it);
        r_t_list.push_back(r_t);
    
    }
    return r_t_list; // Assuming you want to return the average of r_t_list
};

/**
 * @brief Computes the variance for a given vector of log returns and average return.
 * 
 * @param log_return_for_time_period A vector of log returns for the time period.
 * @param average_return_for_time_period The average return for the same period.
 * @return The variance of the log returns.
 */
double iter_variance(const std::vector<double>& log_return_for_time_period, double average_return_for_time_period) {
        double variance_result = 0.0;
        int count = 0;
        for (std::vector<double>::const_iterator it = log_return_for_time_period.begin(); it != log_return_for_time_period.end(); ++it) {
            variance_result = std::pow((*it - average_return_for_time_period), 2.0) + variance_result;
            count++;
        };

        return variance_result;

};


/**
 * @brief Calculates the volatility for a given time period.
 * 
 * @param log_return A vector of log returns for the time period.
 * @param average The average return for the time period.
 * @return The volatility (standard deviation) of the log returns.
 */
double volatility(std::vector<double>& log_return, double average) {
    double variance = iter_variance(log_return, average)/(log_return.size() - 1);
    double volatility = std::sqrt(variance);
    return volatility;
};

/**
 * @brief Updates the volatility using the Exponentially Weighted Moving Average (EWMA) formula.
 * 
 * @param old_volatility The previous volatility value.
 * @param new_price The new stock price.
 * @param old_price The old stock price.
 * @param lambda The smoothing parameter for EWMA.
 * @return The updated volatility.
 */
double update_volatility(double old_volatility, double new_price, double old_price, double lambda) {
    // Calculate the log return
    double r_t = log(new_price / old_price);
    
    // Update variance using EWMA formula
    double old_variance = std::pow(old_volatility, 2.0);
    double new_variance = (1.0 - lambda) * std::pow(r_t, 2.0) + lambda * old_variance;
    
    // Return the updated volatility (square root of variance)
    return std::sqrt(new_variance);
};

/**
 * @brief Calculates the volatility for a stock over a given time period using an algorithm.
 * 
 * @param stock_prices A vector of stock prices over the given period.
 * @return The calculated volatility.
 */
double volatility_algorithm(std::vector<double>& stock_prices) {
        std::vector<double> log_returns = logarithmic_return_function(stock_prices);
        double avg_return = average_return(log_returns);
        double vol = volatility(log_returns, avg_return);
        return vol;
};


} // namespace VolatilityFunctions