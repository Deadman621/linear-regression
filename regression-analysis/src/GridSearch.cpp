#include "hyperparameteroptimization.h"
#include<regression.h>
#include<vector>



    // Implement the logic to iterate over all combinations of hyperparameters
    // For each combination, train your model_ and evaluate it
    // Keep track of the best set of hyperparameters
    //lowest error
    // Return the best learning rate
    // Return the best number of epochs
    


std::pair<double, double>  HyperParameteroptimization::GridSearch(std::vector<double> LearningRate_Values,std::vector<double> epochs_values){
   
    double best_learningrate = LearningRate_Values[0];
    double best_epochs = epochs_values[0];
    double error = model_.MeanSquaredError(dataset_);
    std::vector<double> predictions;
    
    for (int i=0;i<LearningRate_Values.size();i++) {
        for (int j=0;j<epochs_values.size();j++) {
            
            model_.SetLearningRate(LearningRate_Values[i]);

           
            model_.Train(epochs_values[j],100,false); 

           
            predictions = model_.Predict(dataset_); 

            double next_error = model_.MeanSquaredError(dataset_);
            if(next_error < error){
                error = next_error;
                best_learningrate = LearningRate_Values[i];
                best_epochs = epochs_values[j];
            }
            
        }
    }

    return {best_learningrate, best_epochs};
}