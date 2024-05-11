#include<array>
#include<vector>
#include<cmath>
#include<tuple>
#include<Data.h>
#include<ostream>
#include<iostream>
#include<stdexcept>
#include<regression.h>
//#include<matplot/matplot.h>

using namespace std;

void checkNaN(double value, const string& errorMessage) {
    if (isnan(value)) 
        throw invalid_argument{errorMessage};
}

double RegressionEquation(const Model& model, size_t data_point) {
    double y = 0;
    for(size_t i = 0; i < model.numFeatures; i++) 
        y += model.m[i] * model.x.at(data_point)[i];
    return y + model.b;
}

double RegressionEquation(const Model& model, const vector<double>& x) {
    if (x.size() == 0) 
        throw invalid_argument{"Empty dataset"};
    if (model.m.size() == 0)
        throw invalid_argument{"Empty weights"};
    double y = 0;
    for(size_t i = 0; i < x.size(); i++) 
        y += model.m[i] * x[i];
    return y + model.b;
}

Model::Model(Data d, double learning_rate) 
    : data{d}, b{0}, error{0}, 
      learning_rate{learning_rate},
      numBatches{0} 
{
    tie(this->x, this->y) = data.getTrainingData();

    if (y.empty()) 
        throw std::runtime_error("dataset is empty");

    for(auto const& data_point : x) 
        if (x.empty())
            throw runtime_error{"datasets are empty"};

    this->numFeatures = x[0].size();
    this->numDataPoints = y.size();

    m = vector<double>(numFeatures, 0.0);

}

void Model::SetLearningRate(double rate) { this->learning_rate = rate; }

double Model::GetLearningRate(void) const noexcept { return this->learning_rate; }

double Model::MeanSquaredError(size_t start_index, size_t batch_size) { 
    this->error = 0;
    size_t end_index = min(start_index + batch_size, this->y.size());
    size_t n = end_index - start_index;
    for (size_t i = start_index; i < end_index; i++) 
        this->error += pow((this->y[i] - RegressionEquation(*this, i)), 2);
    this->error /= n;

    return this->error;
}

// Check Error on Testing Dataset
double Model::MeanSquaredError(const Data& d) const {
    double Error = 0;
    vector<vector<double>> x;
    vector<double> y;
    tie(x, y) = d.getTestingData();
    for (int i = 0; i < y.size(); i++) 
        Error += pow((y[i] - RegressionEquation(*this, x[i])), 2);
    Error /= y.size();

    return Error;
}

double Model::MeanAbsolutePercentageError(const Data& d) const {
    double Error = 0;
    vector<vector<double>> x;
    vector<double> y;
    tie(x, y) = d.getTestingData();
    for (int i = 0; i < y.size(); i++) {
        double prediction = RegressionEquation(*this, x[i]);
        Error += abs((y[i] - prediction) / y[i]);
    }
    Error /= y.size();

    return Error * 100;  // Return as percentage
}

void Model::GradientDescent(size_t start_index, size_t batch_size) {
    vector<double> m_gradient(numFeatures, 0.0);
    double b_gradient = 0;
    size_t end_index = min(start_index + batch_size, this->y.size());
    size_t n = end_index - start_index;

    for (size_t i = start_index; i < end_index; i++) {
        double prediction = RegressionEquation(*this, i);
        for(size_t j = 0; j < numFeatures; j++) 
            m_gradient[j] += -(2.0 / n) * x[i][j] * (y[i] - prediction);

        b_gradient += -(2.0 / n) * (y[i] - prediction);
    }

    for(size_t i = 0; i < m.size(); i++) 
        m[i] -= m_gradient[i] * learning_rate;
    b -= b_gradient * learning_rate;
}

void Model::Train(size_t epochs, size_t batch_size, bool display_batch) {
    if ((batch_size >= numDataPoints) || (batch_size == 0))
        throw invalid_argument{"Invalid batch size"};

    for (size_t epoch = 0; epoch < epochs; epoch++) {
        for (size_t i = 0; i < numDataPoints; i += batch_size) {
            size_t current_batch_size = min(batch_size, numDataPoints - i);
            GradientDescent(i, current_batch_size);
            MeanSquaredError(i, current_batch_size);
        }

        if (epoch % 500 == 0 && display_batch) 
            cout << "Epoch: " << epoch << " Error: " << error << endl;
    }
}

void Model::DisplayPlot(void) {
/*     vector<double> x0, x1;

    for (const auto& data_point : x) {
        x0.push_back(data_point[0]);
        if (numFeatures == 2) 
            x1.push_back(data_point[1]);
    }

    if (numFeatures == 1) {
        vector<double> x_line = matplot::linspace(matplot::min(x0), matplot::max(x0), numDataPoints);
        vector<double> y_line = matplot::transform(x_line, [this](auto x_line) { return m[0] * x_line + b; });

        matplot::figure();
        matplot::hold(matplot::on);
        matplot::scatter(x0, y)->color("black").marker(".").fill(true).marker_size(10);
        matplot::plot(x_line, y_line)->color("black").line_width(2.0);
        matplot::show();
        cin.get();

    } else if (numFeatures == 2) {
        vector<double> x_line = matplot::linspace(matplot::min(x0), matplot::max(x0), numDataPoints);
        vector<double> y_line = matplot::linspace(matplot::min(x1), matplot::max(x1), numDataPoints);
        vector<double> z_line = matplot::transform(x_line, y_line, [this](double x, double y) { return m[0]*x + m[1]*y + b; });

        matplot::figure();
        matplot::scatter3(x0, x1, y)->color("green").marker_size(10);
        matplot::hold(matplot::on);
        matplot::plot3(x_line, y_line, z_line)->color("black").line_width(2.0);
        matplot::view(-30, 1);
        matplot::show();
        cin.get();
    } */
}

vector<double> Model::Predict(const Data& d) const {
    vector<double> predictions;
    vector<vector<double>> test = d.getTestingData().first;
    for (size_t i = 0; i < test.size(); i++) 
        predictions.push_back(RegressionEquation(*this, test[i]));

    return predictions;
}

ostream& operator<<(ostream& output, const Model& model) {
    output << endl << "Weights: " << endl;
    for(size_t i = 0; i < model.m.size(); i++) 
        output << "m" << i << ": " << model.m[i] << endl;
    output << "Bias: " << model.b << endl;

    return output;
}

bool Model::operator==(const Model& model) const {
    return { 
        (this->m == model.m) && 
        (this->b == model.b) && 
        (this->x == model.x) && 
        (this->y == model.y) && 
        (this->data == model.data) &&
        (this->numFeatures == model.numFeatures) &&
        (this->numDataPoints == model.numDataPoints) &&
        (this->error == model.error) &&
        (this->learning_rate == model.learning_rate)
    }; 
} 

Model& Model::operator=(const Model& model) {
    if (this == &model) 
        return *this;

    this->m = model.m;
    this->b = model.b;
    this->x = model.x;
    this->y = model.y;
    this->data = model.data;
    this->numFeatures = model.numFeatures;
    this->numDataPoints = model.numDataPoints;
    this->error = model.error;
    this->learning_rate = model.learning_rate;

    return *this;
}

vector<double> Model::operator()(const vector<vector<double>>& x) const {
    vector<double> predictions;
    for (size_t i = 0; i < x.size(); i++) 
        predictions.push_back(RegressionEquation(*this, x[i]));

    return predictions;
}

vector<double> Model::operator()(const Data& d) const {
    return Predict(d);
}