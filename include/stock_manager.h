#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility> // For std::pair

/**
 * @brief Calculates the percentage changes in stock prices.
 * 
 * This function computes the percentage change between consecutive prices for each stock.
 * 
 * @param ticker_to_prices A map of stock tickers to their price vectors over time.
 * @return A map of stock tickers to their percentage change vectors.
 */
std::map<std::string, std::vector<double>> calculate_percentage_changes(
    const std::map<std::string, std::vector<double>>& ticker_to_prices) {
    
    // Map to store percentage changes for each ticker
    std::map<std::string, std::vector<double>> ticker_to_percentage_changes;

    // Iterate through each ticker and its price vector
    for (const auto& [ticker, prices] : ticker_to_prices) {
        std::vector<double> percentage_changes;

        // Calculate percentage changes for this ticker
        for (size_t i = 1; i < prices.size(); ++i) {
            double prev_price = prices[i - 1];
            double curr_price = prices[i];

            if (prev_price != 0) { // Avoid division by zero
                double percentage_change = ((curr_price - prev_price) / prev_price) * 100.0;
                percentage_changes.push_back(percentage_change);
            } else {
                percentage_changes.push_back(0.0); // No change if previous price is zero
            }
        }
        

        // Add this ticker's percentage changes to the map
        ticker_to_percentage_changes[ticker] = percentage_changes;
    }

    return ticker_to_percentage_changes;
}

/**
 * @struct Stock_Manager_Result
 * @brief Holds the results of stock management operations.
 * 
 * Contains lists of stocks to buy or sell and reallocation funds available at each hour.
 */
struct Stock_Manager_Result {
    std::vector<std::vector<std::string>> buying_stocks;   // List of stocks to buy at each hour
    std::vector<std::vector<std::string>> selling_stocks;  // List of stocks to sell at each hour
    std::vector<double> reallocation_funds;                // Funds freed up at each hour
};

/**
 * @brief Manages stock buying and selling decisions based on strategy and volatility data.
 * 
 * This function determines which stocks to buy or sell and calculates the funds 
 * available for reallocation based on a chosen investment strategy and stock volatility.
 * 
 * @param stocks A map of stock tickers to their volatility vectors over time.
 * @param my_portfolio A reference to the current portfolio, mapping stock tickers to invested amounts.
 * @param strategy The investment strategy ("optimistic", "neutral", or "conservative").
 * @return A Stock_Manager_Result object containing the buying, selling decisions, and reallocation funds.
 */
Stock_Manager_Result stock_manager(
    const std::map<std::string, std::vector<double>>& stocks,
    std::map<std::string, double>& my_portfolio,
    const std::string& strategy) {
    
    Stock_Manager_Result result;

    // Determine the maximum number of hours based on any stock's volatility vector
    size_t max_hours = 0;
    for (const auto& [stock, volatility_values] : stocks) {
        max_hours = std::max(max_hours, volatility_values.size());
    }

    // Process each hour
    for (size_t hour = 0; hour < max_hours; ++hour) {
        std::vector<std::string> buying_stocks_hour;
        std::vector<std::string> selling_stocks_hour;
        double reallocation_funds_hour = 0.0;

        for (const auto& [stock, volatility_values] : stocks) {
            double& invested_money = my_portfolio[stock];
            double adjustment = 0.0;

            // Get the volatility for the current hour, defaulting to the last value if out of bounds
            double avg_volatility = hour < volatility_values.size() ? volatility_values[hour] : volatility_values.back();

                // Adjustments based on the strategy and average volatility
                if (strategy == "optimistic") {
                    // "Optimistic" strategy focuses on more buying opportunities, even at higher volatility.
                    if (avg_volatility <= 0.0025) {
                        buying_stocks_hour.push_back(stock); // Strong buy
                    } else if (avg_volatility <= 0.004) {
                        buying_stocks_hour.push_back(stock); // Moderate buy
                    } else {
                        // Very high volatility; sell a portion of the stock to free up funds
                        adjustment = -invested_money * 0.05; // Light sell
                        reallocation_funds_hour -= adjustment; // Add funds
                        selling_stocks_hour.push_back(stock);
                    }
                } else if (strategy == "neutral") {
                    // "Neutral" strategy balances between buying and selling.
                    if (avg_volatility > 0.004) {
                        adjustment = -invested_money * 0.03; // Light sell for higher volatility
                        reallocation_funds_hour -= adjustment; // Free up funds
                        selling_stocks_hour.push_back(stock);
                    } else if (avg_volatility > 0.003) {
                        // Moderate volatility; no action or slight buy
                        buying_stocks_hour.push_back(stock);
                    } else {
                        // Low volatility; slight buy
                        buying_stocks_hour.push_back(stock);
                    }
                } else if (strategy == "conservative") {
                    // "Conservative" strategy is cautious about high volatility.
                    if (avg_volatility > 0.004) {
                        adjustment = -invested_money * 0.1; // Strong sell for very high volatility
                        reallocation_funds_hour -= adjustment; // Free up significant funds
                        selling_stocks_hour.push_back(stock);
                    } else if (avg_volatility > 0.0035) {
                        adjustment = -invested_money * 0.05; // Moderate sell
                        reallocation_funds_hour -= adjustment;
                        selling_stocks_hour.push_back(stock);
                    } else {
                        // Low volatility; slight buy
                        buying_stocks_hour.push_back(stock);
                    }
                }
                

            // Update the portfolio based on adjustment
            invested_money += adjustment;
        }

        // Save results for this hour
        result.buying_stocks.push_back(buying_stocks_hour);
        result.selling_stocks.push_back(selling_stocks_hour);
        result.reallocation_funds.push_back(reallocation_funds_hour);
    }

    return result;
}