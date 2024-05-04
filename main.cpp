#include<iostream>
#include<iomanip>
#include<vector>
#include<random>
#include<algorithm>
#include<regression.h>
#include<Data.h>
#include<stdexcept>

using namespace std;

int main(void) {

    try {

        Data data{"D:\\basp\\C++\\Implementing Linear Regression For Predictive Analysis\\Datasets\\Salary_Data.csv"};
        data.InitializeTrainingData(0.8);
        Model model{data};
        model.SetLearningRate(0.01);
        model.Train(1000, true, 500);
        vector<double> predicted = model(data);
        cout << endl;
        for (int i = 0; i < data.getTestingData().second.size(); i++) 
            cout << "Predicted: " << predicted[i] << setw(10) << "Actual: " << data.getTestingData().second[i] << endl;
        cout << "Error: " << model.MeanSquaredError(data);
        cout << endl << model;
        model.DisplayPlot();

    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }

    return 0;
}