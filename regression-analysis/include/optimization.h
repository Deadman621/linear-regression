#include <iostream>
#include <vector>
#include <Data.h>
#include <tuple>

#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H
class Optimization {
    public:
        virtual double GridSearch(std::vector<double> LearningRate_Values,std::vector<double> epochs_values,std::vector<DataPoint> DP, bool N=false, bool S=false) = 0;
        virtual double RandomSearch(std::vector<double> LearningRate_range,std::vector<double> epochs_range,std::vector<DataPoint> DP, bool N=false, bool S=false) = 0;
};

#endif