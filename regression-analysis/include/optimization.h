#include<iostream>

#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

template<typename T>
class Optimization {
    public:
        virtual double optimizeLearningRate(T& model) = 0;  
        virtual int optimizeEpochs(T& model) = 0;
};

#endif