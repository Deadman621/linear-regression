#include<iomanip>
#include<regression.h>

using namespace std;

int main(void) {

    try {

        Data data{"D:\\basp\\C++\\Implementing Linear Regression For Predictive Analysis\\Datasets\\Student_Performance.csv"};
        SaveModel s{"D:\\basp\\C++\\Implementing Linear Regression For Predictive Analysis\\Trained Models\\"};

        data.InitializeTrainingData(0.99);
        Model model{data};

/*         model = s.Load("Student_Performance");
        cout << model; */

        model.Train(true);

        vector<double> predicted = model(data);

        model.DisplayPlot();
        for (int i = 0; i < data.getTestingData().second.size(); i++) 
            cout << "Predicted: " << predicted[i] << setw(10) << "Actual: " << data.getTestingData().second[i] << endl;
        cout << "Error: " << model.MeanAbsolutePercentageError(data) << '%';
        cout << endl << model;

        s.Save(model, "Student_Performance");

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