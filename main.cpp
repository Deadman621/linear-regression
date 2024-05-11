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

        Data data{"D:\\basp\\C++\\Implementing Linear Regression For Predictive Analysis\\Datasets\\Salary_Data.csv"};
        data.InitializeTrainingData(0.8);
        Model model{data};
/*         model.SetLearningRate(0.0001);
        cout << endl;
        model.Train(100000, 5, false);
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