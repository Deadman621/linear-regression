#include<Analysis.h>
#include<stdexcept>

using namespace std;

vector<double> AnalysisTools::Mean(const vector<vector<double>>& data) {
    if(data.empty() || data[0].empty()){
        throw invalid_argument("Cannot calculate mean of empty data set");
    }
    vector<double> sum(data[0].size(), 0);
    for (const auto& row : data) {
        for (int i = 0; i < row.size(); i++) {
            sum[i] += row[i];
        }
    }
    for (double& s : sum) {
        s /= data.size();
    }
    return sum;
}

double AnalysisTools::Mean(const vector<double>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate mean of empty data set");
    }
    double sum = 0;
    for (const auto& value : data) {
        sum += value;
    }
    return sum / data.size();
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


vector<double> AnalysisTools::Variance(const vector<vector<double>>& data) {
    if(data.empty() || data[0].empty()){
        throw invalid_argument("Cannot calculate variance of empty data set");
    }
    vector<double> means = Mean(data);
    vector<double> variances(data[0].size(), 0);
    for (const auto& row : data) {
        for (int i = 0; i < row.size(); i++) {
            variances[i] += (row[i] - means[i]) * (row[i] - means[i]);
        }
    }
    for (double& variance : variances) {
        variance /= data.size();
    }
    return variances;
}

vector<double> AnalysisTools::StandardDeviation(const vector<vector<double>>& data) {
    if(data.empty() || data[0].empty()){
        throw invalid_argument("Cannot calculate standard deviation of empty data set");
    }
    vector<double> variances = Variance(data);
    for (double& variance : variances) {
        variance = sqrt(variance);
    }
    return variances;
}

vector<double> AnalysisTools::Median(const vector<vector<double>>& data) {
    if(data.empty() || data[0].empty()){
        throw invalid_argument("Cannot calculate median of empty data set");
    }
    vector<double> medians(data[0].size(), 0);
    for (int i = 0; i < data[0].size(); i++) {
        vector<double> columnData;
        for (int j = 0; j < data.size(); j++) {
            columnData.push_back(data[j][i]);
        }
        sort(columnData.begin(), columnData.end());
        if (columnData.size() % 2 == 0) {
            medians[i] = (columnData[columnData.size()/2 - 1] + columnData[columnData.size()/2]) / 2;
        } else {
            medians[i] = columnData[columnData.size()/2];
        }
    }
    return medians;
}

pair<vector<double>, double> AnalysisTools::Mean(const vector<DataPoint>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate mean of empty data set");
    }
    vector<vector<double>> x;
    vector<double> y;
    for (const auto& dp : data) {
        x.push_back(dp.getX());
        y.push_back(dp.getY());
    }
    return {AnalysisTools::Mean(x), AnalysisTools::Mean(y)};
}

pair<vector<double>, double> AnalysisTools::Variance(const vector<DataPoint>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate variance of empty data set");
    }
    vector<vector<double>> x;
    vector<double> y;
    for (const auto& dp : data) {
        x.push_back(dp.getX());
        y.push_back(dp.getY());
    }
    return {AnalysisTools::Variance(x), AnalysisTools::Variance(y)};
}

pair<vector<double>, double> AnalysisTools::StandardDeviation(const vector<DataPoint>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate standard deviation of empty data set");
    }
    vector<vector<double>> x;
    vector<double> y;
    for (const auto& dp : data) {
        x.push_back(dp.getX());
        y.push_back(dp.getY());
    }
    return {AnalysisTools::StandardDeviation(x), AnalysisTools::StandardDeviation(y)};
}

pair<vector<double>, double> AnalysisTools::Median(const vector<DataPoint>& data) {
    if(data.empty()){
        throw invalid_argument("Cannot calculate median of empty data set");
    }
    vector<vector<double>> x;
    vector<double> y;
    for (const auto& dp : data) {
        x.push_back(dp.getX());
        y.push_back(dp.getY());
    }
    return {AnalysisTools::Median(x), AnalysisTools::Median(y)};
}

pair<vector<double>, double> AnalysisTools::Range(const vector<DataPoint>& data) {
    if(data.empty() || data[0].getX().empty()){
        throw invalid_argument("Cannot calculate range of empty data set");
    }
    vector<double> MinX(data[0].getX().size(), data[0].getX(0));
    vector<double> MaxX(data[0].getX().size(), data[0].getX(0));
    double MinY = data[0].getY();
    double MaxY = data[0].getY();
    for (const auto& dp : data) {
        for (int j = 0; j < dp.getX().size(); j++) {
            if (dp.getX(j) < MinX[j]) {
                MinX[j] = dp.getX(j);
            }
            if (dp.getX(j) > MaxX[j]) {
                MaxX[j] = dp.getX(j);
            }
        }
        if (dp.getY() < MinY) {
            MinY = dp.getY();
        }
        if (dp.getY() > MaxY) {
            MaxY = dp.getY();
        }
    }
    vector<double> rangeX;
    for (int i = 0; i < MinX.size(); i++) {
        rangeX.push_back(MaxX[i] - MinX[i]);
    }
    return {rangeX, MaxY - MinY};
}

pair<vector<double>, double> AnalysisTools::Min(const vector<DataPoint>& data) {
    if(data.empty() || data[0].getX().empty()){
        throw invalid_argument("Cannot calculate min of empty data set");
    }
    vector<double> MinX(data[0].getX().size(), data[0].getX(0));
    double MinY = data[0].getY();
    for (const auto& dp : data) {
        for (int j = 0; j < dp.getX().size(); j++) {
            if (dp.getX(j) < MinX[j]) {
                MinX[j] = dp.getX(j);
            }
        }
        if (dp.getY() < MinY) {
            MinY = dp.getY();
        }
    }
    return {MinX, MinY};
}

pair<vector<double>, double> AnalysisTools::Max(const vector<DataPoint>& data) {
    if(data.empty() || data[0].getX().empty()){
        throw invalid_argument("Cannot calculate max of empty data set");
    }
    vector<double> MaxX(data[0].getX().size(), data[0].getX(0));
    double MaxY = data[0].getY();
    for (const auto& dp : data) {
        for (int j = 0; j < dp.getX().size(); j++) {
            if (dp.getX(j) > MaxX[j]) {
                MaxX[j] = dp.getX(j);
            }
        }
        if (dp.getY() > MaxY) {
            MaxY = dp.getY();
        }
    }
    return {MaxX, MaxY};
}