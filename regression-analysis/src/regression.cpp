#include<array>
#include<vector>
#include<Data.h>
#include<ostream>
#include<iostream>
#include<stdexcept>
#include<regression.h>
#include<matplot/matplot.h>

using namespace std;

void checkNaN(double value, const string& errorMessage) {
    if (isnan(value)) 
        throw invalid_argument{errorMessage};
}

double RegressionEquation(const Model& model, size_t data_point) {
    double y = 0;
    for(size_t i = 0; i < model.numFeatures; i++) 
        y += model.m[i] * model.x[data_point][i];
    return y + model.b;
}

double RegressionEquation(const Model& model, const vector<double>& x) {
    double y = 0;
    for(size_t i = 0; i < x.size(); i++) 
        y += model.m[i] * x[i];
    return y + model.b;
}

Model::Model(Data d, double learning_rate) 
    : data{d}, b{0}, error{0}, 
      learning_rate{learning_rate} 
{
    this->x = data.getTrainingX();
    this->y = data.getTrainingY();

    if (y.empty() || x.empty()) 
        throw std::runtime_error("dataset is empty");

    this->numFeatures = x.size();
    this->numDataPoints = y.size();

    m = vector<double>(numFeatures, 0.0);
    
    vector<double> x_min(numFeatures), x_max(numFeatures);

    for(size_t i = 0; i < numFeatures; i++) {
        x_min[i] = *min_element(x[i].begin(), x[i].end());
        x_max[i] = *max_element(x[i].begin(), x[i].end());
    }


    double y_min = *min_element(y.begin(), y.end());
    double y_max = *max_element(y.begin(), y.end());

    input_range = {x_min, x_max};
    output_range = {y_min, y_max};
}

void Model::SetLearningRate(double rate) { this->learning_rate = rate; }

double Model::GetLearningRate(void) const { return this->learning_rate; }

double Model::MeanSquaredError(void) { 
    this->error = 0;
    for (int i = 0; i < this->numDataPoints; i++) 
        this->error += pow((this->y[i] - RegressionEquation(*this, i)), 2);
    this->error /= this->numDataPoints;

    return this->error;
}

double Model::MeanSquaredError(const Data& d) const {
    double Error = 0;
    for (int i = 0; i < y.size(); i++) 
        Error += pow((y[i] - RegressionEquation(*this, x[i])), 2);
    Error /= y.size();

    return Error;
}

void Model::GradientDescent(void) {
    vector<double> m_gradient(numFeatures, 0.0);
    double b_gradient = 0;

    for (int i = 0; i < numDataPoints; i++) {
        double prediction = RegressionEquation(*this, i);
        for(size_t j = 0; j < numFeatures; j++) 
            m_gradient[j] += -2 * x[i][j] * (y[i] - prediction);

        b_gradient += -2 * (y[i] - prediction);
    }

    for(size_t i = 0; i < m.size(); i++) 
        m[i] -= (m_gradient[i] / numDataPoints) * learning_rate;
    b -= (b_gradient / numDataPoints) * learning_rate;
}

void Model::Train(int epochs, bool display_batch, int batch_size) {
    for (int i = 0; i < epochs; i++) {
        GradientDescent();
        MeanSquaredError();
        if (display_batch && (i % batch_size == 0)) 
            cout << "Epoch: " << i+1 << " Error: " << error << endl;
        
    }
}

void Model::DisplayPlot(void) {

    if (numFeatures == 1) {
            vector<double> x_line = {input_range[0][0], input_range[1][0]};
            vector<double> y_line = {m[0]*x_line[0] + b, m[0]*x_line[1] + b};

            matplot::figure();
            matplot::scatter(x[0], y);
            matplot::hold(matplot::on);
            matplot::plot(x_line, y_line);
            matplot::show();

    } else if (numFeatures == 2) {

            vector<double> x_line = {input_range[0][0], input_range[1][0]};
            vector<double> y_line = {input_range[0][1], input_range[1][1]};
            vector<double> z_line = {m[0]*x_line[0] + m[1]*y_line[0] + b, m[0]*x_line[1] + m[1]*y_line[1] + b};

            matplot::figure();
            matplot::scatter3(x[0], x[1], y);
            matplot::hold(matplot::on);
            matplot::plot3(x_line, y_line, z_line);
            matplot::show();
    }

}

vector<double> Model::Predict(const Data& d) {
    vector<double> predictions;
    vector<vector<double>> test = d.getTestingX();
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