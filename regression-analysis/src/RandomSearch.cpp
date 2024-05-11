#include "hyperparameteroptimization.h"
#include <random>


    // Implement the logic to randomly sample hyperparameters from the ranges
    // For each sample, train your model and evaluate it
    // Keep track of the best set of hyperparameters
    // Return the best learning rate
      // Return the best number of epochs

  std::pair<double, double> HyperParameteroptimization::RandomSearch(std::vector<double> LearningRate_range,std::vector<double> epochs_range){
     

      std::random_device rd;
      std::mt19937 gen(rd());

      
      std::uniform_int_distribution<> lr(0, LearningRate_range.size() - 1);
      std::uniform_int_distribution<> ep(0, epochs_range.size() - 1);

      double best_learningrate = LearningRate_range[lr(gen)];

      double best_epochs = epochs_range[ep(gen)];
      model_.SetLearningRate(best_learningrate);
      model_.Train(best_epochs,1, false);
      double best_error = model_.MeanSquaredError(dataset_);

  
    for (int i = 0; i < 20; ++i) { 
        double learningrate = LearningRate_range[lr(gen)];
        

        double epochs = epochs_range[ep(gen)];
        
        model_.SetLearningRate(learningrate);
        model_.Train(epochs,1, false);

        double error = model_.MeanSquaredError(dataset_);
        
        if (error < best_error) {
            best_learningrate = learningrate;
            best_epochs = epochs;
            best_error = error;
        }
    }

    return {best_learningrate, best_epochs};
  };
