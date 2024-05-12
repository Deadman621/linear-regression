#include<regression.h>
#include<optimization.h>
#include<Data.h>
#include<Analysis.h>

#ifndef HYPERPARAMETER_OPTIMIZATION_H
#define HYPERPARAMETER_OPTIMIZATION_H


class HyperParameteroptimization : public Optimization { 
public:
    HyperParameteroptimization( Data& dataset,bool N=false, bool S=false)
        :  dataset_(dataset), model_{dataset,0.0001,N,S} { 
           
    };
    std::tuple<double, double, int> GridSearch(std::vector<double> LearningRate_Values,std::vector<double> epochs_values,std::vector<DataPoint> DP, bool N=false, bool S=false) ;
    std::tuple<double, double, int> RandomSearch(std::vector<double> LearningRate_range,std::vector<double> epochs_range,std::vector<DataPoint> DP, bool N=false, bool S=false) ;
private:
    Model model_;
    Data& dataset_; 
};

#endif