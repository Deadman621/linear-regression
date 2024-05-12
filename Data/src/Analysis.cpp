#include <Analysis.h>
#include<stdexcept>

using namespace std;

double AnalysisTools::Mean(const vector<double>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate mean of empty data set");
    }
    double sum = 0;
    for (double num : data) 
        sum += num;
    
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

pair<double, double> AnalysisTools::Mean(const vector<DataPoint>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate mean of empty data set");
    }
    vector<vector<double>> x;
    vector<double> y;
    for (int i = 0; i < data.size(); i++) {
        x.push_back(data[i].getX());
        y.push_back(data[i].getY());
    }
    return {AnalysisTools::Mean(x), AnalysisTools::Mean(y)};
}

pair<double, double> AnalysisTools::Variance(const std::vector<DataPoint>& data){
    if(data.empty()){
        throw invalid_argument("Cannot calculate variance of empty data set");
    }
    vector<vector<double>> x;
    vector<double> y;
    for (int i = 0; i < data.size(); i++) {
        x.push_back(data[i].getX());
        y.push_back(data[i].getY());
    }
    return {AnalysisTools::Variance(x), AnalysisTools::Variance(y)};
}

pair<double, double> AnalysisTools::StandardDeviation(const std::vector<DataPoint>& data){
    if(data.empty()){
        throw invalid_argument("Cannot calculate standard deviation of empty data set");
    }
    vector<vector<double>> x;
    vector<double> y;
    for (int i = 0; i < data.size(); i++) {
        x.push_back(data[i].getX());
        y.push_back(data[i].getY());
    }
    return {AnalysisTools::StandardDeviation(x), AnalysisTools::StandardDeviation(y)};
}

pair<double, double> AnalysisTools::Median(const std::vector<DataPoint>& data){
    if(data.empty()){
        throw invalid_argument("Cannot calculate median of empty data set");
    }
    vector<vector<double>> x;
    vector<double> y;
    for (int i = 0; i < data.size(); i++) {
        x.push_back(data[i].getX());
        y.push_back(data[i].getY());
    }
    return {AnalysisTools::Median(x), AnalysisTools::Median(y)};
}

pair<double, double> AnalysisTools::Range(const std::vector<DataPoint>& data){
    if(data.empty()){
        throw invalid_argument("Cannot calculate range of empty data set");
    }
    double MinX = data[0].getX(0);
    double MaxX = data[0].getX(0);
    double MinY = data[0].getY();
    double MaxY = data[0].getY();
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].getX().size(); j++) {
            if (data[i].getX(j) < MinX) {
                MinX = data[i].getX(j);
            }
            if (data[i].getX(j) > MaxX) {
                MaxX = data[i].getX(j);
            }
        }
        if (data[i].getY() < MinY) {
            MinY = data[i].getY();
        }
        if (data[i].getY() > MaxY) {
            MaxY = data[i].getY();
        }
    }
    return {MaxX - MinX, MaxY - MinY};
}

pair<double, double> AnalysisTools::Min(const std::vector<DataPoint>& data){
    if(data.empty()){
        throw invalid_argument("Cannot calculate min of empty data set");
    }
    double MinX = data[0].getX(0);
    double MinY = data[0].getY();
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].getX().size(); j++) {
            if (data[i].getX(j) < MinX) {
                MinX = data[i].getX(j);
            }
        }
        if (data[i].getY() < MinY) {
            MinY = data[i].getY();
        }
    }
    return {MinX, MinY};
}

pair<double, double> AnalysisTools::Max(const std::vector<DataPoint>& data){
    if(data.empty()){
        throw invalid_argument("Cannot calculate max of empty data set");
    }
    double MaxX = data[0].getX(0);
    double MaxY = data[0].getY();
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].getX().size(); j++) {
            if (data[i].getX(j) > MaxX) {
                MaxX = data[i].getX(j);
            }
        }
        if (data[i].getY() > MaxY) {
            MaxY = data[i].getY();
        }
    }
    return {MaxX, MaxY};
}