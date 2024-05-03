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

    try{

        Data data{"D:\\basp\\C++\\Implementing Linear Regression For Predictive Analysis\\Datasets\\Data.csv"};
        data.InitializeTrainingData(0.8);
        RegressionModel model{data};
        model.SetLearningRate(0.01);
        model.Train(100000, true, 500);
        vector<double> predicted = model.Predict(data);
        cout << endl;
        for (int i = 0; i < data.getTestingY().size(); i++) 
            cout << "Predicted: " << predicted[i] << setw(10) << "Actual: " << data.getTestingY()[i] << endl;
        cout << "Error: " << model.MeanSquaredError(data);
        cout << endl << model;
        model.DisplayPlot();

    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }

    return 0;
}
