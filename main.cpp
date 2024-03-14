#include<iostream>
#include<iomanip>
#include<regression.h>

using namespace std;

int main(void) {

    vector<double> x = {0.54, 8.75, 4.58, 9.30, 3.20, 8.37, 0.61, 8.10, 8.42, 7.85, 9.94, 7.22, 2.83, 4.21, 5.63};
    vector<double> y = {3.83, 18.99, 9.83, 21.67, 8.09, 20.03, 4.21, 18.58, 20.25, 19.88, 22.42, 16.44, 7.06, 10.20, 12.22};

    vector<double> testx{3.78, 1.00, 5.39, 7.55};
    vector<double> testy{8.87, 4.59, 11.76, 18.33};

    LinearRegression model{x, y};
    model.SetLearningRate(0.0001);
    model.Train(100000);
    vector<double> predicted = model.Predict(testx);
    for (int i = 0; i < testx.size(); i++) 
        cout << "Predicted: " << predicted[i] << setw(10) << "Actual: " << testy[i] << endl;
    cout << "Error: " << model.MeanSquaredError(testx, testy);

    return 0;
}