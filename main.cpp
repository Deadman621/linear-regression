#include<iostream>
#include<iomanip>
#include<vector>
#include<random>
#include<algorithm>
#include<regression.h>

using namespace std;

int main(void) {
    vector<vector<double>> trainingFeatures = {
        {1.0, 2.0},
        {2.0, 3.0},
        {3.0, 4.0},
        {4.0, 5.0},
        {5.0, 6.0}
    };

    vector<double> trainingLabels = {3.0, 5.0, 7.0, 9.0, 11.0};

    vector<vector<double>> testingFeatures = {
        {6.0, 7.0},
        {7.0, 8.0},
        {8.0, 9.0},
        {9.0, 10.0},
        {10.0, 11.0}
    };

    vector<double> testingLabels = {13.0, 15.0, 17.0, 19.0, 21.0};

    Model model{trainingFeatures, trainingLabels, 2, 5};
    model.SetLearningRate(0.01);
    model.Train(1000);
    vector<double> predicted = model.Predict(testingFeatures);
    for (int i = 0; i < testingFeatures.size(); i++) 
        cout << "Predicted: " << predicted[i] << setw(10) << "Actual: " << testingLabels[i] << endl;
    cout << "Error: " << model.MeanSquaredError(testingFeatures, testingLabels);
    cout << endl << model;

    return 0;
}