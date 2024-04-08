#include<vector>
#include<array>
#include<ostream>

#ifndef REGRESSION_H
#define REGRESSION_H

using namespace std;
class Model {
    private:
        double b;
        double error;
        double learning_rate;
        size_t numFeatures;
        size_t numDataPoints;
        vector<double> m;
        vector<vector<double>> x;
        vector<double> y;
        array<vector<double>, 2> input_range;
        array<double, 2> output_range;
        void GradientDescent(void);

    friend double RegressionEquation(const Model& model, size_t data_point);
    friend double RegressionEquation(const Model& model, const vector<double>& x);
    friend ostream& operator<<(ostream& output, const Model& model);

    public:
        Model(vector<vector<double>> InputValues, vector<double> OutputValues, size_t numFeatures, size_t numDataPoints, double learning_rate = 0.0001);
        double MeanSquaredError(void);
        double MeanSquaredError(vector<vector<double>> x, vector<double> y) const;
        void SetLearningRate(double rate);
        double GetLearningRate(void) const; 
        void Train(int epochs, bool display_batch = false, int batch_size = 0);
        void DisplayPlot(void);
        vector<double> Predict(vector<vector<double>> x);
};

#endif