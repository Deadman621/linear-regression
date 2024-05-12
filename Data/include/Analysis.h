#include <vector>
#include <algorithm>
#include <cmath>
#include <Data.h>

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
    static std::pair<double, double> Min(const std::vector<DataPoint>& data);
    static std::pair<double, double> Max(const std::vector<DataPoint>& data);
    static std::pair<double, double> Mean(const std::vector<DataPoint>& data);
    static std::pair<double, double> Variance(const std::vector<DataPoint>& data);
    static std::pair<double, double> StandardDeviation(const std::vector<DataPoint>& data);
    static std::pair<double, double> Median(const std::vector<DataPoint>& data);
    static std::pair<double, double> Range(const std::vector<DataPoint>& data);

    friend class Data;
};
#endif