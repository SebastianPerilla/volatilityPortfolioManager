#include <vector>

namespace volFormula {

    /**
     * @brief Calculates the average of a vector of doubles.
     *
     * @param return_list A vector containing the data points.
     * @return The average value of the data points.
     */
    // Return datatype: double
    double average(std::vector<double> &returnList);

    /**
     * @brief Computes the average return for a given time period.
     *
     * @param r_average_list A vector of returns for the given period.
     * @return The average return.
     */
    double averageReturn(std::vector<double> r_average_list);

    /**
     * @brief Computes the logarithmic returns for a given price vector.
     *
     * @param price A vector of stock prices over time.
     * @return A vector of logarithmic returns.
     */
    std::vector<double> logarithmicReturnFunction(std::vector<double> &price);

    /**
     * @brief Computes the variance for a given vector of log returns and average return.
     *
     * @param log_return_for_time_period A vector of log returns for the time period.
     * @param average_return_for_time_period The average return for the same period.
     * @return The variance of the log returns.
     */
    double iterVariance(const std::vector<double> &logReturnTimePeriod, double averageReturnForTimePeriod);

    /**
     * @brief Calculates the volatility for a given time period.
     *
     * @param log_return A vector of log returns for the time period.
     * @param average The average return for the time period.
     * @return The volatility (standard deviation) of the log returns.
     */
    double volatility(std::vector<double> &log_return, double average);

    /**
     * @brief Updates the volatility using the Exponentially Weighted Moving Average (EWMA) formula.
     *
     * @param old_volatility The previous volatility value.
     * @param new_price The new stock price.
     * @param old_price The old stock price.
     * @param lambda The smoothing parameter for EWMA.
     * @return The updated volatility.
     */
    double update_volatility(double old_volatility, double new_price, double old_price, double lambda);
    /**
     * @brief Calculates the volatility for a stock over a given time period using an algorithm.
     *
     * @param stock_prices A vector of stock prices over the given period.
     * @return The calculated volatility.
     */
    double volatilityAlgorithm(std::vector<double> &stock_prices);

} // namespace volFormula
