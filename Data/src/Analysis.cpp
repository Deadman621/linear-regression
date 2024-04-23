#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> // for sort
#include <cmath> // for sqrt

using namespace std;

class AnalysisTools {
    public:
    static double Mean(const vector<double> &data) {
        double sum = 0;
        for (double num : data) {
            sum += num;
        }
        return data.empty() ? 0 : sum / data.size();
    }
    static double Variance(const vector<double> &data) {
        double mean = Mean(data);
        double sum = 0;
        for (int i = 0; i < data.size(); i++) {
            sum += (data[i] - mean) * (data[i] - mean);
        }
        return data.empty() ? 0 : (sum / (data.size() - 1));
    }
    static double StandardDeviation(const vector<double> &data) {
        return sqrt(Variance(data));
    }
    static double Median(const vector<double> &data) {
        if(data.empty()){
            return 0;
        }
        vector <double> copy = data;
        std::sort(&copy[0], &copy[0] + copy.size());
        if (copy.size() % 2 == 0) {
            return (copy[copy.size() / 2] + copy[(copy.size() / 2) - 1]) / 2;
        } else {
            return copy[copy.size() / 2];
        }
    }
    static double Mean(const vector<vector<double>> &data) {
        vector<double> means;
        for (int i = 0; i < data.size(); i++) {
            means.push_back(Mean(data[i]));
        }
        return Mean(means);
    }
    static double Variance(const vector<vector<double>> &data) {
        double OverAllMean = Mean(data);
        double sum = 0;
        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < data[i].size(); j++) {
                sum += (data[i][j] - OverAllMean) * (data[i][j] - OverAllMean);
            }
        }
        return data.empty() ? 0 : (sum / (data.size() * data[0].size()));
    }
    static double StandardDeviation(const vector<vector<double>> &data) {
        return sqrt(Variance(data));
    }
    static double Median(const vector<vector<double>> &data) {
        vector<double> medians;
        for (int i = 0; i < data.size(); i++) {
            medians.push_back(Median(data[i]));
        }
        return Median(medians);
    }
};

