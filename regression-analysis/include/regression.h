#include<vector>
#include<array>
#include<ostream>
#include<Data.h>

#ifndef REGRESSION_H
#define REGRESSION_H

#include<Interface.h>

class Model: public ITrainable, public IEvaluable, public IDisplayable {
    private:
        Data data;
        double b;
        double error;
        std::size_t numFeatures;
        std::size_t numDataPoints;
        std::size_t numBatches;
        std::vector<double> m;
        std::vector<double> y;
        std::vector<std::vector<double>> x;
        virtual double MeanSquaredError(size_t start_index, size_t batch_size) override;

    protected:
        double learning_rate;
        void GradientDescent(size_t start_index, size_t batch_size);

    friend double RegressionEquation(const Model& model, size_t data_point);
    friend double RegressionEquation(const Model& model, const std::vector<double>& x);
    friend std::ostream& operator<<(std::ostream& output, const Model& model);

    public:
        Model(Data data, double learning_rate = 0.0001);
        Model(const Model& model);

        virtual double MeanSquaredError(const Data&) const override;
        virtual double MeanAbsolutePercentageError(const Data& d) const override;

        void SetLearningRate(double rate);
        double GetLearningRate(void) const noexcept; 

        void DisplayPlot(void);
        virtual void Train(size_t epochs, size_t batch_size = 50, bool display_batch = false) override;
        std::vector<double> Predict(const Data&) const;

        bool operator==(const Model& model) const;
        Model& operator=(const Model& model);
        std::vector<double> operator()(const std::vector<std::vector<double>>&) const;
        std::vector<double> operator()(const Data&) const;
};

#endif