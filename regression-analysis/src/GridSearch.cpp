#include "hyperparameteroptimization.h"
#include<regression.h>
#include<vector>



    // Implement the logic to iterate over all combinations of hyperparameters
    // For each combination, train your model_ and evaluate it
    // Keep track of the best set of hyperparameters
    //lowest error
    // Return the best learning rate
    // Return the best number of epochs
    


std::tuple<double, double, int>  HyperParameteroptimization::GridSearch(std::vector<double> LearningRate_Values,std::vector<double> epochs_values,std::vector<DataPoint> DP, bool N, bool S){
    
    std::vector<int> batchSize_value = {32, 64, 128, 256, 512};
    double best_learningrate = LearningRate_Values[0];
    double best_epochs = epochs_values[0];
    int bestBatchSize = 1;
    
    double error = model_.MeanSquaredError(dataset_);
    std::vector<double> predictions;
    
    for (int i=0;i<LearningRate_Values.size();i++) {
        for (int j=0;j<epochs_values.size();j++) {
            for(int k=0;k<batchSize_value.size();k++){

                if (batchSize_value[k] > DP.size()) {
                    batchSize_value[k] = DP.size();
                }

                if(LearningRate_Values[i] > 1){
                    throw std::invalid_argument("Learning rate should be less than 1");
                }else if(LearningRate_Values[i] < 0){
                    throw std::invalid_argument("Learning rate should be greater than 0");
                }else{
                    model_.SetLearningRate(LearningRate_Values[i]);
                }
                
                if(epochs_values[j] < 0){
                    throw std::invalid_argument("Epochs should be greater than 0");
                }else{
                    model_.Train(epochs_values[j],(batchSize_value[k] % (DP.size()/2)+1),false); 
                }

            
                predictions = model_.Predict(dataset_);
                double next_error = model_.MeanSquaredError(dataset_);

                if(next_error < error){
                    error = next_error;
                    best_learningrate = LearningRate_Values[i];
                    best_epochs = epochs_values[j];
                    bestBatchSize = batchSize_value[k];

                }

            }
        }
    }

    return {best_learningrate, best_epochs, (bestBatchSize%DP.size())+1};
}