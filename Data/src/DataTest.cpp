#include<iostream>

using namespace std;



int main(void) {
    Data data("Data.csv");
    cout << data;



    double TrainingPer = 0.6;
    data.InitializeTrainingData(TrainingPer); //first call this 

    pair<vector<vector<double>>, vector<double>> TrainingData = data.getTrainingData(); // then this 
    vector <vector<double>> x = TrainingData.first;
    vector <double> y = TrainingData.second;

    cout << "\n\nTraining data :-\n";
    for (int i = 0; i < x.size(); i++) {
        for (int j = 0; j < x[i].size(); j++) {
            cout << x[i][j] << ", ";
        }
        cout << " | " << y[i] << endl;
    }

    pair<vector<vector<double>>, vector<double>> TestingData = data.getTestingData(); // then this
    vector<vector<double>> X = TestingData.first;
    vector<double> Y = TestingData.second;

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