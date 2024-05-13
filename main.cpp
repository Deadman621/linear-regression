#include<iomanip>
#include<regression.h>

using namespace std;

int main(void) {

    try {
        const string path = "D:\\basp\\C++\\Implementing Linear Regression For Predictive Analysis\\";
        
        Data data{path + "Datasets\\Data.csv", 0.1};
        SaveModel s{path + "Trained Models\\"};

        data.InitializeTrainingData(0.8);
        Model model{data};

        //model.Train();
        model = s.Load("Student_Performance");
        vector<double> predicted = model(data);

        model.DisplayPlot();
        cout << endl;
        for (int i = 0; i < data.getTestingData().second.size(); i++) 
            cout << "Predicted: " << predicted[i] << setw(10) << "Actual: " << data.getTestingData().second[i] << endl;
        cout << "Error: " << model.MeanAbsolutePercentageError(data) << '%';

        //s.Save(model, "Student_Performance");

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