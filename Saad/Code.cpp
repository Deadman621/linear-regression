#include "Data.h"
int main() {
    Data data("Data.csv");
    data.DisplayData();



    double TrainingPer = 0.6;
    data.InitializeTrainingData(TrainingPer); //first call this 


    vector <vector<double>> x = data.getTrainingX();
    vector<double> y = data.getTrainingY();

    cout << "\n\nTraining data :-\n";
    for (int i = 0; i < x.size(); i++) {
        for (int j = 0; j < x[i].size(); j++) {
            cout << x[i][j] << ", ";
        }
        cout << " | " << y[i] << endl;
    }


    
    vector<vector<double>> X = data.getTestingX();
    vector<double> Y = data.getTestingY();
    
    cout << "\n\nTesting data :-\n";
    for (int i = 0; i < X.size(); i++) {
        for (int j = 0; j < X[i].size(); j++) {
            cout << X[i][j] << ", ";
        }
        cout << " | " << Y[i] << endl;
    }

    // cout << "\n\nTraining data :- " << endl;
    // vector<DataPoint> ForTraining = data.getTrainingDataPoints(); // then this
    // for(int i = 0 ; i < ForTraining.size() ; i++){
    //     ForTraining[i].Display();
    // }
    
    // cout << "\n\nTesting data :- " << endl;
    // vector<DataPoint> ForTesting = data.getTestingDataPoints(); // or this
    // for(int i = 0 ; i < ForTesting.size() ; i++){
    //     ForTesting[i].Display();
    // }

    return 0;
}