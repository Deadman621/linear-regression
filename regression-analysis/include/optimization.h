#include <iostream>
#include <vector>

#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H


class Optimization {
    public:
        virtual std::pair<double, double>  GridSearch(std::vector<double> LearningRate_Values,std::vector<double> epochs_values) = 0;
        virtual std::pair<double, double>  RandomSearch(std::vector<double> LearningRate_range,std::vector<double> epochs_range) = 0;
        virtual ~Optimization() = default;
};

#endif