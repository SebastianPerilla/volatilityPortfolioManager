#include "volatilityFormula.h"
#include <numeric>

namespace VolatilityFunctions {

    double average(std::vector<double> &returnList) {
        double avg = std::accumulate(returnList.begin(), returnList.end(), 0.0) / returnList.size();
        return avg;
    }

    double averageReturn(std::vector<double> returnAverageList) {
        double rBar = average(returnAverageList);
        return rBar;
    }

    std::vector<double> logarithmicReturnFunction(std::vector<double> &price) {
        std::vector<double> rTlist;

        // Using a regular iterator to loop through all the initial prices
        for (std::vector<double>::iterator it = price.begin(); it != price.end() - 1; ++it) {
            double r_t = log(*(it + 1) / *it);
            rTlist.push_back(r_t);
        }
        return rTlist; // Assuming you want to return the average of r_t_list
    };

    double iterVariance(const std::vector<double> &logReturnTimePeriod, double avgReturnForTimePeriod) {
        double varianceResult = 0.0;
        int count = 0;
        for (std::vector<double>::const_iterator it = logReturnTimePeriod.begin(); it != logReturnTimePeriod.end();
             ++it) {
            varianceResult = std::pow((*it - avgReturnForTimePeriod), 2.0) + varianceResult;
            count++;
        };

        return varianceResult;
    };

    double volatility(std::vector<double> &logReturn, double average) {
        double variance = iterVariance(logReturn, average) / (logReturn.size() - 1);
        double volatility = std::sqrt(variance);
        return volatility;
    };

    double updateVolatility(double oldVol, double newPrice, double oldPrice, double lambda) {
        // Calculate the log return
        double r_t = log(newPrice / oldPrice);

        // Update variance using EWMA formula
        double old_variance = std::pow(oldVol, 2.0);
        double new_variance = (1.0 - lambda) * std::pow(r_t, 2.0) + lambda * old_variance;

        // Return the updated volatility (square root of variance)
        return std::sqrt(new_variance);
    };

} // namespace VolatilityFunctions
