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

        Data data{"D:\\basp\\C++\\Implementing Linear Regression For Predictive Analysis\\Datasets\\Student_Performance.csv"};
        data.InitializeTrainingData(0.99);
        Model model{data};
        /*
        std::vector<double> LearningRate_range = {0.0001, 0.001, 0.01, 0.1, 1};
        std::vector<double> epochs_range = {10, 100, 1000, 10000};
        HyperParameteroptimization optimizer{model, data};
        std::pair<double, double> best_hyperparameters = optimizer.RandomSearch( LearningRate_range, epochs_range);
        model.SetLearningRate(best_hyperparameters.first);
        model.Train(best_hyperparameters.second, 256, true);*/

        /*std::vector<double> LearningRate_Values = {0.0001, 0.001, 0.01, 0.1, 1};
        std::vector<double> epochs_values = {10, 100, 1000, 10000};
        HyperParameteroptimization optimizer{model, data};
        std::pair<double, double> best_hyperparameters = optimizer.GridSearch( LearningRate_Values, epochs_values);
        model.SetLearningRate(best_hyperparameters.first);
        model.Train(best_hyperparameters.second, 256, true);*/

        

/*         model.SetLearningRate(0.0001);
        cout << endl;
        model.Train(10, 256, true); */
        Save<Model> save{"D:\\basp\\C++\\Implementing Linear Regression For Predictive Analysis\\Trained Models\\"};
        save.LoadModel(model, "Student_Performance");
        vector<double> predicted = model(data);
        cout << endl;
        //model.DisplayPlot();
        for (int i = 0; i < data.getTestingData().second.size(); i++) 
            cout << "Predicted: " << predicted[i] << setw(10) << "Actual: " << data.getTestingData().second[i] << endl;
        cout << "Error: " << model.MeanSquaredError(data);
        cout << endl << model;

    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }

    return 0;
}