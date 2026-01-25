#include "volatility_formula.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cmath>

using namespace VolatilityFunctions;


/**
 * @brief Computes hourly volatility for each stock ticker based on the first 6 data points.
 * 
 * This function iterates through the input map of tickers and their associated prices,
 * computes the hourly volatility for the first 6 price points using the volatility algorithm,
 * and stores the results in a new map.
 * 
 * @param input_map A map where the key is the ticker (string) and the value is a vector of stock prices (doubles).
 * @return A map with the ticker as the key and the calculated volatility as the value.
 */
std::map<std::string, double> ticker_to_vol_hourly(std::map<std::string, std::vector<double>> input_map) {
    
    std::map<std::string, double> ticker_vol_map;

    for (const auto& pair : input_map) {
        const std::string& ticker = pair.first;
        const std::vector<double>& prices = pair.second;

        if (prices.size() < 6) {
            std::cout << " Not enough data for " << ticker << std::endl;
            continue;
        }

        // Use only the first 6 values
        std::vector<double> first_six_prices(prices.begin(), prices.begin() + 6);

        // Calculation of Volatility
        double vol_algo = volatility_algorithm(first_six_prices);

        // Adding the value to the initial map for the past volatility calculations
        ticker_vol_map[ticker] = vol_algo;


        // // Print Statements incase you want to checkout the values for yourself

        // std::cout << "\n Ticker: " << ticker << std::endl;
        // std::cout << " Average return: " << avg_return << std::endl;

        // std::cout << "\n Log Returns: " << std::endl;
        // std::cout << " Size: " << log_returns.size() << std::endl;

        // // Prints the log returns
        // for (auto& i : log_returns) {
        //     std::cout << " " << (std::pow(i  - avg_return, 2.0)) << "\n";

        // }

        // std::cout << std::endl;
        // std::cout << " Variances: " << iter_variance(log_returns, avg_return)  << std::endl;
        // std::cout << " Volatility: " << vol_algo << std::endl;
    
    };
    
    return ticker_vol_map;
}

/**
 * @brief Computes the true volatility of stock tickers over time using the Exponentially Weighted Moving Average (EWMA) method.
 * 
 * This function calculates the true volatility for each stock ticker in the input map
 * based on its price history and an initial volatility value.
 * 
 * @param input_map A map where the key is the ticker (string) and the value is a vector of stock prices (doubles).
 * @param standard_ticker_vol_map A map where the key is the ticker and the value is the initial volatility (double) for that ticker.
 * @return A map where the key is the ticker and the value is a vector of volatilities over time.
 */
std::map<std::string, std::vector<double>> true_volatility(std::map<std::string, std::vector<double>> input_map, std::map<std::string, double> standard_ticker_vol_map){
    
    std::cout << "\n-----------------------------------\n";

    std::map<std::string, std::vector<double>> true_volatility_output;
            
    for (const auto& pair : standard_ticker_vol_map) {
        const std::string& ticker = pair.first;
        const std::vector<double>& prices = input_map[ticker];

        if (prices.size() > 6) {
            double current_volatility = pair.second;
            double lambda = 0.94;

            for (size_t i = 5; i < prices.size()-1; ++i) {
                double old_price = prices[i];
                double new_price = prices[i + 1];

                current_volatility = update_volatility(current_volatility, new_price, old_price, lambda);
                
                // std::cout << current_volatility << std::endl;
                true_volatility_output[ticker].push_back(current_volatility);
            }
        } else {
            std::cout << ticker << ": Not enough data" << std::endl;
        }
    }

    return true_volatility_output;
};
