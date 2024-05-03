#include <Analysis.h>
#include<stdexcept>

using namespace std;

double AnalysisTools::Mean(const vector<double>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate mean of empty data set");
    }
    double sum = 0;
    for (double num : data) {
        sum += num;
    }
    return data.empty() ? 0 : sum / data.size();
}

double AnalysisTools::Variance(const vector<double>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate variance of empty data set");
    }
    double mean = Mean(data);
    double sum = 0;
    for (int i = 0; i < data.size(); i++) {
        sum += (data[i] - mean) * (data[i] - mean);
    }
    return  (sum / (data.size() - 1));
}

double AnalysisTools::StandardDeviation(const vector<double>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate standard deviation of empty data set");
    }
    return sqrt(Variance(data));
}

double AnalysisTools::Median(const vector<double>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate median of empty data set");
    }
    vector<double> copy = data;
    sort(copy.begin(), copy.end());
    if (copy.size() % 2 == 0) {
        return (copy[copy.size() / 2] + copy[(copy.size() / 2) - 1]) / 2;
    } else {
        return copy[copy.size() / 2];
    }
}

double AnalysisTools::Mean(const vector<vector<double>>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate mean of empty data set");
    }
    vector<double> means;
    for (int i = 0; i < data.size(); i++) {
        means.push_back(Mean(data[i]));
    }
    return Mean(means);
}

double AnalysisTools::Variance(const vector<vector<double>>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate variance of empty data set");
    }
    double OverAllMean = Mean(data);
    double sum = 0;
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            sum += (data[i][j] - OverAllMean) * (data[i][j] - OverAllMean);
        }
    }
    return data.empty() ? 0 : (sum / (data.size() * data[0].size()));
}

double AnalysisTools::StandardDeviation(const vector<vector<double>>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate standard deviation of empty data set");
    }
    return sqrt(Variance(data));
}

double AnalysisTools::Median(const vector<vector<double>>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate median of empty data set");
    }
    vector<double> medians;
    for (int i = 0; i < data.size(); i++) {
        medians.push_back(Median(data[i]));
    }
    return Median(medians);
}