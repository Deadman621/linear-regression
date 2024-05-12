#include "hyperparameteroptimization.h"
#include<regression.h>
#include<vector>



    // Implement the logic to iterate over all combinations of hyperparameters
    // For each combination, train your model_ and evaluate it
    // Keep track of the best set of hyperparameters
    //lowest error
    // Return the best learning rate
    // Return the best number of epochs
    


double HyperParameteroptimization::GridSearch(std::vector<double> LearningRate_Values,std::vector<double> epochs_values,std::vector<DataPoint> DP, bool N, bool S){
    
    std::vector<int> batchSize_value = {32, 64, 128, 256, 512};
    best_learningrate = LearningRate_Values[0];
    best_epochs = epochs_values[0];
    bestBatchSize = DP.size() / 2;

    //this->best_error = model_->MeanSquaredError(dataset_);
    std::vector<double> predictions;
    
    for (int i=0;i<LearningRate_Values.size();i++) {
        for (int j=0;j<epochs_values.size();j++) {
            for(int k=0;k<batchSize_value.size();k++){

                if (batchSize_value[k] > DP.size()) {
                    batchSize_value[k] = DP.size();
                }

                if(LearningRate_Values[i] > 1){
                    throw std::invalid_argument("Learning rate should be less than 1 in GridSearch");
                }else if(LearningRate_Values[i] < 0){
                    throw std::invalid_argument("Learning rate should be greater than 0 in GridSearch");
                }else{
                    model_->SetLearningRate(LearningRate_Values[i]);
                }
                
                if(epochs_values[j] < 0){
                    throw std::invalid_argument("Epochs should be greater than 0 in GridSearch");
                }else{
                    if (DP.size() > 1) {
                        size_t bs = (batchSize_value[k] % (DP.size()/2)+1);
                        model_->Train(epochs_values.at(j),bs);
                    } else {
                        throw std::runtime_error("Dataset is empty in GridSearch");
                    } 
                }

            
                predictions = model_->Predict(dataset_);
                double next_error = model_->MeanSquaredError(dataset_);

                if(next_error < best_error){
                    best_error = next_error;
                    best_learningrate = LearningRate_Values[i];
                    best_epochs = epochs_values[j];
                    bestBatchSize = batchSize_value[k];

                }

            }
        }
    }

    bestBatchSize = (bestBatchSize%DP.size())+1;

    return best_error;
}