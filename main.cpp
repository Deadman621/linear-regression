#include<iostream>
#include<iomanip>
#include<vector>
#include<random>
#include<algorithm>
#include<regression.h>
#include<Data.h>
#include<Analysis.h>
#include<HyperParameterOptimization.h>
#include<stdexcept>

using namespace std;

int main(void) {

    try {

        Data data{"C:\\Users\\Muddassir\\OneDrive\\Desktop\\project\\linear-regression\\Datasets\\3Dataset.csv"};
        data.InitializeTrainingData(0.8);
        Model model{data};

          
        HyperParameteroptimization optimizer{ data};
        vector<double> LearningRate_range{0.0001, 0.001, 0.01, 0.1, 1};
        vector<double> epochs_range{100, 500, 1000, 5000, 10000};
        
        pair<double, double> best_hyperparameters = optimizer.RandomSearch(LearningRate_range, epochs_range);
        cout<<"Best Learning Rate: "<<best_hyperparameters.first<<endl;
        cout<<"Best Epochs: "<<best_hyperparameters.second<<endl;
        model.SetLearningRate(best_hyperparameters.first);
        model.Train(best_hyperparameters.second, 1, false);   
        
      /* vector<double> LearningRate_Values{0.0001, 0.001, 0.01, 0.1, 1};
        vector<double> epochs_values{100, 500, 1000, 5000, 10000};
        pair<double, double> best_hyperparameters = optimizer.GridSearch(LearningRate_Values, epochs_values);   
        model.SetLearningRate(best_hyperparameters.first);
        model.Train(best_hyperparameters.second, 100, false);   */


        
/*         Save<Model> save{"D:\\basp\\C++\\Implementing Linear Regression For Predictive Analysis\\Trained Models\\"};
        save.LoadModel(model, "Student_Performance"); */
        vector<double> predicted = model(data);
        cout << endl;
        model.DisplayPlot();
        for (int i = 0; i < data.getTestingData().second.size(); i++) 
            cout << "Predicted: " << predicted[i] << setw(10) << "Actual: " << data.getTestingData().second[i] << endl;
        cout << "Error: " << model.MeanSquaredError(data);
        cout << endl << model;

    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }

    return 0;
}