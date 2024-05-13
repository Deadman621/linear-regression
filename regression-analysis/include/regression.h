#include<Data.h>

#ifndef REGRESSION_H
#define REGRESSION_H

#include<Interface.h>

class HyperParameteroptimization;

class Model: public ITrainable, public IEvaluable, public IDisplayable {
    private:
        Data data;
        HyperParameteroptimization* optimizer = nullptr;
        double b;
        double error;
        std::size_t numFeatures;
        std::size_t numDataPoints;
        std::size_t numBatches;
        std::vector<double> m;
        std::vector<double> y;
        std::vector<std::vector<double>> x;
        virtual double MeanSquaredError(size_t start_index, size_t batch_size) override;
        void Optimize(void);

    protected:
        double learning_rate;
        void GradientDescent(size_t start_index, size_t batch_size);

    friend double RegressionEquation(const Model& model, size_t data_point);
    friend double RegressionEquation(const Model& model, const std::vector<double>& x);
    friend std::ostream& operator<<(std::ostream& output, const Model& model);
    friend class SaveModel;

    public:
        Model(Data& data);
        Model(const Model& model);

        virtual double MeanSquaredError(const Data& d) const override;
        virtual double MeanAbsolutePercentageError(const Data& d) const override;

        void SetLearningRate(double rate);
        double GetLearningRate(void) const noexcept; 

        void DisplayPlot(void);
        virtual void Train(bool display_batch = false) override;
        virtual void Train(size_t epochs, size_t batch_size, bool display_batch = false) override;

        std::vector<double> Predict(const Data&) const;

        bool operator==(const Model& model) const;
        Model& operator=(const Model& model);
        std::vector<double> operator()(const std::vector<std::vector<double>>&) const;
        std::vector<double> operator()(const Data&) const;

        ~Model(void);
};

class SaveModel: public iSaveable<Model> {
    private:
        std::string path;

    public:
        SaveModel(const std::string& path): path{path} {}
        // Name of Folder in Current Workspace
        void SetSaveDestination(const std::string& saveDestination) { this->path = saveDestination; }
        void Save(const Model& model, const std::string& filename) override;
        Model Load(const std::string& filename) override;
        bool Exists(const std::string& filename) override;
        void Delete(const std::string& filename) override;
};

#endif