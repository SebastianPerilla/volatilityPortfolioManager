#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
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
    size_t writeCallBack(void *contents, size_t size, size_t nmemb, std::string *userp) {
        userp->append((char *)contents, size * nmemb);
        return size * nmemb;
    }

    /**
     * @brief Converts a date string to a Unix timestamp.
     *
     * The function converts a date in the format "YYYY-MM-DD" into a Unix timestamp.
     *
     * @param date The date string to convert.
     * @return The Unix timestamp representation of the date.
     */
    long convertToTimestamp(const std::string &date) {
        std::tm tm = {};
        std::istringstream ss(date);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        return std::mktime(&tm);
    }

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
    void getStockData(const std::string &ticker, const std::string &startDate, const std::string &endDate,
                      std::map<std::string, std::vector<double>> &tickerToPrices) {
        CURL *curl = curl_easy_init();
        if (!curl) {
            std::cerr << "Failed to initialize CURL" << std::endl;
            return;
        }

        long period1 = convertToTimestamp(startDate);
        long period2 = convertToTimestamp(endDate);

        std::string url = "https://query1.finance.yahoo.com/v8/finance/chart/" + ticker +
                          "?period1=" + std::to_string(period1) + "&period2=" + std::to_string(period2) +
                          "&interval=1h";

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string response_data;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Failed to fetch data: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            return;
        }

        try {
            json data = json::parse(response_data);

            if (!data["chart"]["result"][0]["timestamp"].is_null()) {
                auto timestamps = data["chart"]["result"][0]["timestamp"];
                auto prices = data["chart"]["result"][0]["indicators"]["quote"][0]["close"];

                for (size_t i = 0; i < timestamps.size(); i++) {
                    if (!prices[i].is_null()) {
                        tickerToPrices[ticker].push_back(static_cast<double>(prices[i]));
                    }
                }

                std::cout << "Data for " << ticker << " has been processed and stored." << std::endl;
            }
        } catch (const json::exception &e) {
            std::cerr << "JSON error: " << e.what() << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    /**
     * @brief Saves stock data to a CSV file.
     *
     * This function writes stock ticker symbols and their corresponding prices
     * to a CSV file.
     *
     * @param filename The name of the CSV file to save the data.
     * @param ticker_to_prices The map containing ticker symbols and their prices.
     */
    void saveToCsv(const std::string &filename, const std::map<std::string, std::vector<double>> &ticker_to_prices) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        file << "ticker,price\n";
        file << std::fixed << std::setprecision(20);
        for (const auto &[ticker, prices] : ticker_to_prices) {
            for (const auto &price : prices) {
                file << ticker << "," << price << "\n";
            }
        }
        file.close();

        std::cout << "Data has been saved to " << filename << std::endl;
    }

    // Function to print a std::map (used for debugging purposes)
    void printMap(const std::map<std::string, double> &myMap, const std::string &title) {
        std::cout << title << std::endl;
        for (const auto &[key, value] : myMap) {
            std::cout << key << ": " << value << std::endl;
        }
        std::cout << std::endl;
    }

} // namespace extractor
