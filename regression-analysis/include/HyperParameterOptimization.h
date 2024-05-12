#include<optimization.h>
#include<regression.h>
#include<limits>

#ifndef HYPERPARAMETER_OPTIMIZATION_H
#define HYPERPARAMETER_OPTIMIZATION_H

class HyperParameteroptimization : public Optimization { 
private:
    HyperParameteroptimization( Data& dataset)
        :  dataset_(dataset) { };
    double RandomSearch(std::vector<double> LearningRate_range,std::vector<double> epochs_range,std::vector<DataPoint> DP, bool N=false, bool S=false) ;
    double GridSearch(std::vector<double> LearningRate_Values,std::vector<double> epochs_values,std::vector<DataPoint> DP, bool N=false, bool S=false) ;

    double best_error = std::numeric_limits<double>::max();
    size_t bestBatchSize;
    double best_learningrate;
    double best_epochs;
    Model* model_;
    Data& dataset_; 
    void setModel(Model* model) { model_ = model; }

    friend class Model;
};

#endif