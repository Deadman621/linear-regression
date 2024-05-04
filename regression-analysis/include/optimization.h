#include<iostream>

// Specify Type of Model to be optimized
template<typename T>
class Optimization {
    public:
        virtual double optimizeLearningRate(T& model) = 0;  
        virtual int optimizeEpochs(T& model) = 0;
        virtual void GradientDescent(void) = 0;  
};