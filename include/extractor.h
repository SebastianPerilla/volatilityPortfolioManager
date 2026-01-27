
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
// #include <curl/curl.h>
// #include <nlohmann/json.hpp>
#include <ctime>
#include <iomanip> // For std::setprecision and std::fixed

// using json = nlohmann::json;

namespace extractor {

    /**
     * @brief Callback function for CURL to write response data.
     *
     * This function appends the response data to a user-provided string.
     *
     * @param contents Pointer to the response data.
     * @param size Size of each data element.
     * @param nmemb Number of elements.
     * @param userp Pointer to the user-provided string to store data.
     * @return The number of bytes processed.
     */
    size_t writeCallBack(void *contents, size_t size, size_t nmemb, std::string *userp);
    /**
     * @brief Converts a date string to a Unix timestamp.
     *
     * The function converts a date in the format "YYYY-MM-DD" into a Unix timestamp.
     *
     * @param date The date string to convert.
     * @return The Unix timestamp representation of the date.
     */
    long convertToTimestamp(const std::string &date);

    /**
     * @brief Fetches stock data and stores prices in a map.
     *
     * This function uses CURL to fetch stock data from Yahoo Finance for a specified
     * ticker and date range, and stores the prices in a map.
     *
     * @param ticker The stock ticker symbol (e.g., "AAPL").
     * @param start_date The start date for data retrieval in "YYYY-MM-DD" format.
     * @param end_date The end date for data retrieval in "YYYY-MM-DD" format.
     * @param ticker_to_prices A reference to a map to store the fetched prices.
     */
    void get_stock_data(const std::string &ticker, const std::string &startDate, const std::string &endDate,
                        std::map<std::string, std::vector<double>> &tickerToPrices);

    /**
     * @brief Saves stock data to a CSV file.
     *
     * This function writes stock ticker symbols and their corresponding prices
     * to a CSV file.
     *
     * @param filename The name of the CSV file to save the data.
     * @param ticker_to_prices The map containing ticker symbols and their prices.
     */
    void saveToCsv(const std::string &filename, const std::map<std::string, std::vector<double>> &tickerToPrices);

    // Function to print a std::map (used for debugging purposes)
    void printMap(const std::map<std::string, double> &myMap, const std::string &title);

} // namespace extractor
