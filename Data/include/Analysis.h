#include <vector>
#include <algorithm>
#include <cmath>

#ifndef ANALYSIS_H
#define ANALYSIS_H

class AnalysisTools {
public:
    static double Mean(const std::vector<double>& data);
    static double Variance(const std::vector<double>& data);
    static double StandardDeviation(const std::vector<double>& data);
    static double Median(const std::vector<double>& data);
    static double Mean(const std::vector<std::vector<double>>& data);
    static double Variance(const std::vector<std::vector<double>>& data);
    static double StandardDeviation(const std::vector<std::vector<double>>& data);
    static double Median(const std::vector<std::vector<double>>& data);
};

#endif