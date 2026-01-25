// // #include "extractor.h"
// // #include "portfolio_manager.h"
// // #include "stock_manager.h"
// #include "volatilityFormula.h"
// // #include "volatility_parse.h"
// #include <algorithm>
// #include <cctype>
// #include <cmath>
// #include <iomanip>
// #include <iostream>
// #include <limits>
// #include <map>
// // #include <matplot/matplot.h>
// #include <numeric>
// #include <string>
// #include <tuple>
// #include <utility>
// #include <vector>
// using namespace matplot;
//
// /**
//  * @brief Initializes the game and sets up initial variables.
//  *
//  * Prompts the user for initial investment, strategy, and duration in months.
//  * Validates the input and applies default values if the user input is invalid.
//  *
//  * @return A tuple containing the initial investment, number of months, and
//  * investment strategy.
//  */
// std::tuple<float, int, std::string> start_game() {
//     std::cout << "Welcome to Stock Shock. Today is 1st of January of 2023. Let's "
//                  "test your investment skills.\n";
//     std::cout << "You will have a series of decisions to make which will affect "
//                  "how your money behaves, so choose wisely!\n";
//
//     // initial investment
//     float initial_investment = 20000;
//     std::cout << "\nFirst off, how much money would you like to invest? (Enter a "
//                  "positive number or type 'you choose'): ";
//     std::string input;
//     getline(std::cin >> std::ws, input);
//     if (input != "you choose") {
//         try {
//             initial_investment = std::stof(input);
//             if (initial_investment <= 0) {
//                 throw std::invalid_argument("Must be positive");
//             }
//         } catch (std::exception &) {
//             std::cout << "Invalid input. Using default of 20,000 euros." << std::endl;
//             initial_investment = 20000;
//         }
//     }
//
//     // number of months
//     int months = 12;
//     std::cout << "\nHow many months would you like to test? (1-12 or type 'you "
//                  "choose'): ";
//     getline(std::cin >> std::ws, input);
//     if (input != "you choose") {
//         try {
//             months = std::stoi(input);
//             if (months < 1 || months > 12) {
//                 throw std::out_of_range("Must be between 1 and 12");
//             }
//         } catch (std::exception &) {
//             std::cout << "Invalid input. Using default of 12 months." << std::endl;
//             months = 12;
//         }
//     }
//
//     // investment strategy
//     std::string strategy = "Neutral";
//     std::cout << "\nWhat investment strategy would you like to use? (Optimistic, "
//                  "Neutral, Conservative, or type 'you choose'):\n";
//     std::cout << "Optimistic: Go big or go home! High-stakes investing with "
//                  "volatile stocks and emerging markets.\n";
//     std::cout << "Neutral: A balanced strategy with growth stocks and blue-chip "
//                  "names for steady growth.\n";
//     std::cout << "Conservative: Low-volatility stocks and steady dividends, "
//                  "focusing on stable companies.\n";
//     std::cout << "Pick your strategy: ";
//     getline(std::cin, input);
//     std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return std::tolower(c); });
//     if (input == "you choose" || (input != "optimistic" && input != "conservative")) {
//         strategy = "neutral";
//     } else if (input == "optimistic") {
//         strategy = "optimistic";
//     } else if (input == "conservative") {
//         strategy = "conservative";
//     }
//
//     return std::make_tuple(initial_investment, months, strategy);
// }
// /**
//  * @brief Creates an initial portfolio allocation.
//  *
//  * Distributes the initial investment equally among the provided tickers.
//  *
//  * @param tickers A vector of stock tickers.
//  * @param initial_investment The total initial investment amount.
//  * @return A map representing the portfolio with tickers as keys and allocated
//  * amounts as values.
//  */
// std::map<std::string, double> create_portfolio(const std::vector<std::string> &tickers, double initial_investment) {
//     std::map<std::string, double> my_portfolio;
//     if (tickers.empty()) {
//         std::cerr << "Error: No tickers provided.\n";
//         return my_portfolio;
//     }
//     double price_per_ticker = initial_investment / tickers.size();
//     for (const auto &ticker : tickers) {
//         my_portfolio[ticker] = price_per_ticker;
//     }
//
//     return my_portfolio;
// }
//
// /**
//  * @brief Calculates the total value of a portfolio.
//  *
//  * Iterates through the portfolio map to calculate the sum of all values.
//  *
//  * @param portfolio A map representing the portfolio.
//  * @return The total value of the portfolio.
//  */
// double calculate_total_portfolio_value(const std::map<std::string, double> &portfolio) {
//     double total_value = 0.0;
//     for (const auto &[stock, value] : portfolio) {
//         total_value += value;
//     }
//     return total_value;
// }
//
// int main() {
//     // INIT GAME
//     float initial_investment;
//     int months;
//     std::string strategy;
//     // strategy = "neutral";
//     // months = 12;
//     // initial_investment = 20000;
//     std::tie(initial_investment, months, strategy) = start_game();
//
//     // GET PRICE PER HOUR -ISMA
//     // Map to store prices for each ticker
//     std::map<std::string, std::vector<double>> ticker_to_prices;
//     std::vector<std::string> tickers = {
//         "NVDA", "AAPL", "MSFT", "AMZN", "GOOGL", "META", "TSLA", "TSM", "AVGO", "ORCL"
//     };
//     for (const auto &ticker : tickers) {
//         get_stock_data(ticker, "2023-12-30", "2024-11-18", ticker_to_prices);
//     }
//
//     // GET PORTFOLIO
//     // Determine initial investment per stock
//     std::map<std::string, double> my_portfolio = create_portfolio(tickers, initial_investment);
//
//     // GET VOLATILITY MAP
//     std::map<std::string, double> output = ticker_to_vol_hourly(ticker_to_prices);
//     std::map<std::string, std::vector<double>> true_vol = true_volatility(ticker_to_prices, output);
//
//     // Calculate percentage changes
//     std::map<std::string, std::vector<double>> ticker_to_percentage_changes =
//         calculate_percentage_changes(ticker_to_prices);
//
//     // Print the initial portfolio
//     std::cout << "Initial Portfolio:\n";
//     for (const auto &[stock, value] : my_portfolio) {
//         std::cout << stock << ": $" << value << "\n";
//     }
//     std::cout << "--------------------------\n";
//
//     // Call Stock_Manager_Result and get results
//     Stock_Manager_Result stock_result = stock_manager(true_vol, my_portfolio, strategy);
//
//     // Call portfolio_manager and get results
//     Portfolio_Manager_Result portfolio_result =
//         portfolio_manager(stock_result.buying_stocks, stock_result.reallocation_funds, my_portfolio, strategy,
//         true_vol,
//                           ticker_to_percentage_changes);
//
//     std::vector<std::map<std::string, double>> portfolio_snapshots;
//
//     // PRINTING RESULTS/PLOT
//     // Print combined results for each hour
//     size_t hours = stock_result.buying_stocks.size();
//     for (size_t hour = 0; hour < hours; ++hour) {
//         std::cout << "Hour " << hour + 1 << " Results:\n";
//
//         // Print the percentage changes for each stock
//         std::cout << "  Stock Price Changes:\n";
//         for (const auto &[stock, percentage_changes] : ticker_to_percentage_changes) {
//             double percentage_change = 0.0;
//             if (hour < percentage_changes.size()) {
//                 percentage_change = percentage_changes[hour];
//                 std::cout << "    " << stock << ": ";
//                 if (percentage_change >= 0) {
//                     std::cout << "+";
//                 }
//                 std::cout << percentage_change << "%\n";
//             } else {
//                 // If no data for this hour, assume no change
//                 std::cout << "    " << stock << ": No data\n";
//             }
//         }
//
//         // Stock Manager Results
//         std::cout << "  Stock Manager Decisions:\n";
//         std::cout << "    Buying: ";
//         for (const auto &stock : stock_result.buying_stocks[hour]) {
//             std::cout << stock << " ";
//         }
//         std::cout << "\n";
//
//         std::cout << "    Selling: ";
//         for (const auto &stock : stock_result.selling_stocks[hour]) {
//             std::cout << stock << " ";
//         }
//         std::cout << "\n";
//
//         std::cout << "    Funds Available for Reallocation: $" << stock_result.reallocation_funds[hour] << "\n";
//
//         // Portfolio Manager Results
//         std::cout << "  How much we bought:\n";
//         if (hour < portfolio_result.allocations.size() && !portfolio_result.allocations[hour].empty()) {
//             for (const auto &[stock, allocated_funds] : portfolio_result.allocations[hour]) {
//                 std::cout << "    - " << stock << ": $" << allocated_funds << "\n";
//             }
//         } else {
//             std::cout << "    No funds allocated this hour.\n";
//         }
//
//         // Print the updated portfolio at the start of the hour
//         std::cout << "  Your Portfolio at the end of this hour:\n";
//         // Use the stored portfolio values for this hour
//         if (hour < portfolio_result.portfolio_values.size()) {
//             const auto &portfolio_at_hour = portfolio_result.portfolio_values[hour];
//             portfolio_snapshots.push_back(portfolio_result.portfolio_values[hour]);
//             for (const auto &[stock, value] : portfolio_at_hour) {
//                 std::cout << "    " << stock << ": $" << value << "\n";
//             }
//         } else {
//             // If for some reason we don't have portfolio values for this hour, print
//             // current my_portfolio
//             portfolio_snapshots.push_back(portfolio_snapshots.back());
//             for (const auto &[stock, value] : my_portfolio) {
//                 std::cout << "    " << stock << ": $" << value << "\n";
//             }
//         }
//         std::cout << "--------------------------\n";
//     }
//
//     // Print final portfolio
//     std::cout << "\nYour Final Portfolio:\n";
//     for (const auto &[stock, value] : my_portfolio) {
//         std::cout << stock << ": $" << value << "\n";
//     }
//
//     // Calculate and print total gain/loss
//     double final_portfolio_value = calculate_total_portfolio_value(my_portfolio);
//     double gain_loss = final_portfolio_value - initial_investment;
//
//     std::cout << "\nTotal Gain/Loss: $";
//     if (gain_loss >= 0) {
//         std::cout << "+";
//     }
//     std::cout << gain_loss << " (" << (gain_loss / initial_investment) * 100 << "%)\n";
//
//     // PLOT the portfolio over time
//     std::map<std::string, std::vector<double>> stock_data;
//     std::vector<double> time_hours(portfolio_snapshots.size());
//
//     for (size_t i = 0; i < portfolio_snapshots.size(); ++i) {
//         time_hours[i] = i;
//         for (const auto &[stock, value] : portfolio_snapshots[i]) {
//             stock_data[stock].push_back(value);
//         }
//     }
//
//     auto ax = gca();
//     hold(ax, on);
//     std::vector<std::string> colors = { "b", "g", "r", "c", "m", "y", "k", "#b", "g", "r" };
//     std::vector<std::string> markers = { "*", "*", "*", "*", "*", "*", "*", "x", "x", "x" };
//     size_t color_index = 0;
//
//     for (const auto &[stock, values] : stock_data) {
//         auto line = plot(time_hours, values);
//         (*line).line_width(2);
//         (*line).color(colors[color_index % colors.size()]);
//         (*line).marker_size(8);
//         (*line).display_name(stock);
//         ++color_index;
//     }
//
//     xlabel("Hour");
//     ylabel("Portfolio value ($)");
//     title("Portfolio Over Hours per Ticker");
//     legend()->location(legend::general_alignment::bottomright);
//     hold(ax, off);
//     show();
//
//     return 0;
// }

int main() { return 0; }
