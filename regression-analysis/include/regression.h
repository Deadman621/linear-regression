#include<vector>
#include<array>
#include<ostream>
#include<Data.h>

#ifndef REGRESSION_H
#define REGRESSION_H

class ITrainable {
    public:
        virtual void Train(int epochs, bool display_batch, int batch_size) = 0;
    };

class IEvaluable {
    public:
        virtual double MeanSquaredError(void) = 0;
        virtual double MeanSquaredError(const Data& d) const = 0;
    };

class IDisplayable {
    public:
        virtual void DisplayPlot(void) = 0;
    };

class Model: public ITrainable, public IEvaluable, public IDisplayable {
    private:
        double b;
        double error;
        double learning_rate;
        std::size_t numFeatures;
        std::size_t numDataPoints;
        std::vector<double> m;
        std::array<std::vector<double>, 2> input_range;
        std::array<double, 2> output_range;
        std::vector<std::vector<double>> x;
        std::vector<double> y;
        Data data;
        void GradientDescent(void);

    friend double RegressionEquation(const Model& model, size_t data_point);
    friend double RegressionEquation(const Model& model, const std::vector<double>& x);
    friend std::ostream& operator<<(std::ostream& output, const Model& model);

    public:
        Model(Data data, double learning_rate = 0.0001);
        virtual double MeanSquaredError(void) override;
        virtual double MeanSquaredError(const Data&) const override;
        void SetLearningRate(double rate);
        double GetLearningRate(void) const; 
        virtual void Train(int epochs, bool display_batch = false, int batch_size = 0) override;
        void DisplayPlot(void);
        std::vector<double> Predict(const Data&, bool plot = false);
};

#endif