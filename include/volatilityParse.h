#include <map>
#include <string>
#include <vector>

namespace volFunctions {

    /*
     * @brief Computes hourly volatility for each stock ticker based on the first 6 data points.
     *
     * This function iterates through the input map of tickers and their associated prices,
     * computes the hourly volatility for the first 6 price points using the volatility algorithm,
     * and stores the results in a new map.
     *
     * @param input_map A map where the key is the ticker (string) and the value is a vector of stock prices (doubles).
     * @return A map with the ticker as the key and the calculated volatility as the value.
     */
    std::map<std::string, double> tickerToVolHourly(std::map<std::string, std::vector<double>> input_map);

    /**
     * @brief Computes the true volatility of stock tickers over time using the Exponentially Weighted Moving Average
     * (EWMA) method.
     *
     * This function calculates the true volatility for each stock ticker in the input map
     * based on its price history and an initial volatility value.
     *
     * @param input_map A map where the key is the ticker (string) and the value is a vector of stock prices (doubles).
     * @param standard_ticker_vol_map A map where the key is the ticker and the value is the initial volatility (double)
     * for that ticker.
     * @return A map where the key is the ticker and the value is a vector of volatilities over time.
     */
    std::map<std::string, std::vector<double>> true_volatility(std::map<std::string, std::vector<double>> input_map,
                                                               std::map<std::string, double> standard_ticker_vol_map);
} // namespace volFunctions
