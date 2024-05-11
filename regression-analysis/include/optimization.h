#include <iostream>
#include <vector>
#include<tuple>

#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H


class Optimization {
    public:
        virtual std::tuple<double, double, int> GridSearch(std::vector<double> LearningRate_Values,std::vector<double> epochs_values, std::vector<DataPoint> DP) = 0;
        virtual std::tuple<double, double, int> RandomSearch(std::vector<double> LearningRate_range,std::vector<double> epochs_range, std::vector<DataPoint> DP) = 0;
};

#endif