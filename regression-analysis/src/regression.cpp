#include<iostream>
#include<regression.h>
#include<vector>
#include<array>
#include<matplot/matplot.h>

using namespace std;

LinearRegression::LinearRegression(vector<double> x, vector<double> y, double learning_rate) 
    : x{x}, y{y}, m{0}, b{0}, error{0}, learning_rate{learning_rate} 
{
    double x_min = *min_element(x.begin(), x.end());
    double x_max = *max_element(x.begin(), x.end());

    double y_min = *min_element(y.begin(), y.end());
    double y_max = *max_element(y.begin(), y.end());

    input_range = {x_min, x_max};
    output_range = {y_min, y_max};
}

void LinearRegression::SetLearningRate(double rate) { this->learning_rate = rate; }
double LinearRegression::GetLearningRate(void) { return this->learning_rate; }

double LinearRegression::MeanSquaredError(void) { 
    this->error = 0;
    for (int i = 0; i < this->y.size(); i++) 
        this->error += pow((this->y[i] - (this->m * this->x[i] + this->b)), 2);
    this->error /= this->y.size();

    return this->error;
}

double LinearRegression::MeanSquaredError(vector<double> x, vector<double> y) {
    double Error = 0;
    for (int i = 0; i < y.size(); i++) 
        Error += pow((y[i] - (m * x[i] + b)), 2);
    Error /= y.size();

    return Error;
}

void LinearRegression::GradientDescent(void) {
    double m_gradient = 0;
    double b_gradient = 0;
    int n = y.size();

    for (int i = 0; i < n; i++) {
        m_gradient += -2 * x[i] * (y[i] - (m * x[i] + b));
        b_gradient += -2 * (y[i] - (m * x[i] + b));
    }

    m -= (m_gradient / n) * learning_rate;
    b -= (b_gradient / n) * learning_rate;
}

void LinearRegression::Train(int epochs) {
    for (int i = 0; i < epochs; i++) {
        GradientDescent();
        MeanSquaredError();
        if (i % 500 == 0)
            cout << "Epoch: " << i+1 << " Error: " << error << endl;
    }
    
    vector<double> input;
    vector<double> output;

    for (double i = input_range[0]; i < input_range[1]; i++) {
        input.push_back(i);
        output.push_back(m * i + b);
    }

    matplot::scatter(x, y);
    matplot::hold(matplot::on); 
    matplot::plot(input, output);
    matplot::show(); 
}

vector<double> LinearRegression::Predict(vector<double> x) {
    vector<double> predictions;
    for (auto const &i : x) 
        predictions.push_back(m * i + b);

    return predictions;
}

void LinearRegression::PrintModel(void) {
    cout << "Slope: " << m << " Intercept: " << b << endl;
    // plot the data and the line using matplot++
    matplot::plot(x, y);
    matplot::show();
}