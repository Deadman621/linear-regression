#include<iostream>
#include<iomanip>
#include<vector>
#include<random>
#include<algorithm>
#include<regression.h>
#include<Data.h>

using namespace std;

int main(void) {

    Data data{"Data.csv"};

    Model model{data, 6, 10};
    model.SetLearningRate(0.01);
    model.Train(1000);
    vector<double> predicted = model.Predict(data);
    for (int i = 0; i < data.getTestingX().size(); i++) 
        cout << "Predicted: " << predicted[i] << setw(10) << "Actual: " << data.getTrainingY()[i] << endl;
    cout << "Error: " << model.MeanSquaredError(data);
    cout << endl << model;

    return 0;
}