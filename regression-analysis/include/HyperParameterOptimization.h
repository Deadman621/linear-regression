#include<regression.h>
#include<optimization.h>
#include<Data.h>
#include<Analysis.h>

#ifndef HYPERPARAMETER_OPTIMIZATION_H
#define HYPERPARAMETER_OPTIMIZATION_H


class HyperParameteroptimization : public Optimization { 
public:
    HyperParameteroptimization( Data& dataset)
        :  dataset_(dataset), model_{dataset} { 
           
    };
    std::pair<double, double>  GridSearch(std::vector<double> LearningRate_Values,std::vector<double> epochs_values);
    std::pair<double, double> RandomSearch(std::vector<double> LearningRate_range,std::vector<double> epochs_range);
private:
    Model model_;
    Data& dataset_; 
};

#endif