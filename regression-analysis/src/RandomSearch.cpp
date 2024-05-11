#include "hyperparameteroptimization.h"
#include <random>


    // Implement the logic to randomly sample hyperparameters from the ranges
    // For each sample, train your model and evaluate it
    // Keep track of the best set of hyperparameters
    // Return the best learning rate
      // Return the best number of epochs

  std::tuple<double, double, int> HyperParameteroptimization::RandomSearch(std::vector<double> LearningRate_range,std::vector<double> epochs_range, std::vector<DataPoint> DP){
     

      std::random_device rd;
      std::mt19937 gen(rd());

      std::vector<int> batchSize_range = {32, 64, 128, 256, 512};
      std::uniform_int_distribution<> bs(0, batchSize_range.size() - 1);
      std::uniform_int_distribution<> lr(0, LearningRate_range.size() - 1);
      std::uniform_int_distribution<> ep(0, epochs_range.size() - 1);

      double best_learningrate = LearningRate_range[lr(gen)];

      double best_epochs = epochs_range[ep(gen)];

      int bestBatchSize;

      model_.SetLearningRate(best_learningrate);
      model_.Train(best_epochs,1, false);
      double best_error = model_.MeanSquaredError(dataset_);

  
    for (int i = 0; i < 32; ++i) { 
        
        double learningrate = LearningRate_range[lr(gen)];
        
        double epochs = epochs_range[ep(gen)];

        int batchSize = (batchSize_range[bs(gen)]) % (DP.size()/2) + 1;
    
        if(learningrate > 1){
            throw std::invalid_argument("Learning rate should be less than 1");
        }else if(learningrate < 0){
            throw std::invalid_argument("Learning rate should be greater than 0");
        }else{
            model_.SetLearningRate(learningrate);
        }

        if(epochs < 0){
            throw std::invalid_argument("Epochs should be greater than 0");
        }else{  
            model_.Train(epochs, batchSize , false);
        }


        double error = model_.MeanSquaredError(dataset_);
        
        if (error < best_error) {
            best_learningrate = learningrate;
            best_epochs = epochs;
            best_error = error;
            bestBatchSize = batchSize;
        }
    }

    return {best_learningrate, best_epochs, bestBatchSize};
  };
