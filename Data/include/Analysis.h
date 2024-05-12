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
        static std::vector<double> Mean(const std::vector<std::vector<double>>& data); 
        static std::vector<double> Variance(const std::vector<std::vector<double>>& data); 
        static std::vector<double> StandardDeviation(const std::vector<std::vector<double>>& data); 
        static std::vector<double> Median(const std::vector<std::vector<double>>& data); 
        static std::pair<std::vector<double>, double> Min(const std::vector<DataPoint>& data); 
        static std::pair<std::vector<double>, double> Max(const std::vector<DataPoint>& data); 
        static std::pair<std::vector<double>, double> Mean(const std::vector<DataPoint>& data); 
        static std::pair<std::vector<double>, double> Variance(const std::vector<DataPoint>& data); 
        static std::pair<std::vector<double>, double> StandardDeviation(const std::vector<DataPoint>& data); 
        static std::pair<std::vector<double>, double> Median(const std::vector<DataPoint>& data); 
        static std::pair<std::vector<double>, double> Range(const std::vector<DataPoint>& data); 

    friend class Data;
};
#endif