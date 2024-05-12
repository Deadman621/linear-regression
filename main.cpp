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
<<<<<<<<< Temporary merge branch 1
        Model model{data, 0.0001,true,false}; // true for Normalization and false for Standardization
        
=========
        Model model{data};
>>>>>>>>> Temporary merge branch 2
          
        HyperParameteroptimization optimizer{data};
        /* vector<double> LearningRate_range{0.000001, 0.00001, 0.0001, 0.001, 0.01,0.1};
        vector<double> epochs_range{100, 500, 1000, 5000, 10000};
        tuple<double, double , int> best_hyperparameters = optimizer.RandomSearch(LearningRate_range, epochs_range, data.getTrainingDataPoints());
        model.SetLearningRate(get<0>(best_hyperparameters));
        model.Train(get<1>(best_hyperparameters) ,get<2>(best_hyperparameters), false);   */
     
        vector<double> LearningRate_Values{0.0001, 0.001, 0.01, 0.1, 1};
        vector<double> epochs_values{100, 500, 1000, 5000, 10000};
        tuple<double, double, int> best_hyperparameters = optimizer.GridSearch(LearningRate_Values, epochs_values, data.getTrainingDataPoints(true,false));   
        model.SetLearningRate(get<0>(best_hyperparameters));
        model.Train(get<1>(best_hyperparameters) ,get<2>(best_hyperparameters), false);  

        //SaveModel s{"D:\\basp\\C++\\Implementing Linear Regression For Predictive Analysis\\Trained Models\\"};
        //s.Save(model, "Student_Performance");
        //Model model = s.Load("Student_Performance");
        vector<double> predicted = model(data);
        cout << endl;
        model.DisplayPlot();
        for (int i = 0; i < data.getTestingData().second.size(); i++) 
            cout << "Predicted: " << predicted[i] << setw(10) << "Actual: " << data.getTestingData(true,false).second[i] << endl; // true for Normalization and false for Standardization
        cout << "Error: " << model.MeanAbsolutePercentageError(data,true,false) << "%" << endl;
        cout << endl << model;
/*         data.DispAllData();
        data.DispNormalizedData(); */
        //data.DispStandardizedData();

    } catch (const std::ios_base::failure& e) {
        cout << "File operation failed: " << e.what() << endl;
    } catch (const invalid_argument& e) {
        cout << e.what() << endl;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    } catch (...) {
        cout << "An unknown error occurred" << endl;
    }

    return 0;
}