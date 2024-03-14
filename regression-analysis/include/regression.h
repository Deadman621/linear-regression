#include<vector>
#include<array>

#ifndef REGRESSION_H
#define REGRESSION_H

class LinearRegression {
    private:
        double m;
        double b;
        double error;
        double learning_rate;
        std::vector<double> x;
        std::vector<double> y;
        std::array<double, 2> input_range;
        std::array<double, 2> output_range;
        void GradientDescent(void);

    public:
        LinearRegression(std::vector<double> InputValues, std::vector<double> OutputValues, double learning_rate = 0.0001);
        double MeanSquaredError(void);
        double MeanSquaredError(std::vector<double> x, std::vector<double> y);
        void SetLearningRate(double rate);
        double GetLearningRate(void); 
        void Train(int epochs);
        std::vector<double> Predict(std::vector<double> x);
        void PrintModel(void);
};

#endif