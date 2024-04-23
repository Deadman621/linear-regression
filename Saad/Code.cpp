#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> // for sort
#include <cmath> // for sqrt

using namespace std;


class AnalysisTools {
    public:
    static double Mean(const vector<double> &data) {
        double sum = 0;
        for (double num : data) {
            sum += num;
        }
        return data.empty() ? 0 : sum / data.size();
    }
    static double Variance(const vector<double> &data) {
        double mean = Mean(data);
        double sum = 0;
        for (int i = 0; i < data.size(); i++) {
            sum += (data[i] - mean) * (data[i] - mean);
        }
        return data.empty() ? 0 : (sum / (data.size() - 1));
    }
    static double StandardDeviation(const vector<double> &data) {
        return sqrt(Variance(data));
    }
    static double Median(const vector<double> &data) {
        if(data.empty()){
            return 0;
        }
        vector <double> copy = data;
        sort(&copy[0], &copy[0] + copy.size());
        if (copy.size() % 2 == 0) {
            return (copy[copy.size() / 2] + copy[(copy.size() / 2) - 1]) / 2;
        } else {
            return copy[copy.size() / 2];
        }
    }
    static double Mean(const vector<vector<double>> &data) {
        vector<double> means;
        for (int i = 0; i < data.size(); i++) {
            means.push_back(Mean(data[i]));
        }
        return Mean(means);
    }
    static double Variance(const vector<vector<double>> &data) {
        double OverAllMean = Mean(data);
        double sum = 0;
        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < data[i].size(); j++) {
                sum += (data[i][j] - OverAllMean) * (data[i][j] - OverAllMean);
            }
        }
        return data.empty() ? 0 : (sum / (data.size() * data[0].size()));
    }
    static double StandardDeviation(const vector<vector<double>> &data) {
        return sqrt(Variance(data));
    }
    static double Median(const vector<vector<double>> &data) {
        vector<double> medians;
        for (int i = 0; i < data.size(); i++) {
            medians.push_back(Median(data[i]));
        }
        return Median(medians);
    }
};

class File {
    protected:
    ifstream file;
    string fileName;
    string Type;
    public:
    File(string n, string t){
        fileName = n;
        Type = t;
    }
    ~File() {
        if(file.is_open()){
            file.close();
        }
    }
};

class CSVFile : public File {
    protected:
    const char separator;
    public:
    CSVFile(string n = "") : File(n, "CSV"), separator(',') {}
};

class DataPoint{

    vector<double> x;
    double y;
    
    public:
    DataPoint(){};
    DataPoint(vector<double> X, double Y){
        x = X;
        y = Y;
    }
    vector<double> getX(){
        return x;
    }
    double getY(){
        return y;
    }
    double getX(int i){
        return x[i];
    }

    void Display(){
        cout << x[0];
        for (int i = 0; i < x.size(); i++) {
            cout << ", " << x[i];
        }
        cout << " | ";
        cout << y << endl;
    }
    friend class Data;
};

class Data : public CSVFile {
    int MaxVariablesQty;
    vector<string> NameOfAllVariables;
    int ColumnIndexForDependentVariable;

    vector<DataPoint> DP;
    vector<DataPoint> Training;
    vector<DataPoint> Testing;
    int NumberOfRows;
    
    public:
    Data(string Name) : CSVFile(Name) {
        file.open(fileName);
        if(!file.is_open()){
            cout << "File not found" << endl;
            exit(1);
        }
        NumberOfRows = 0;
        MaxVariablesQty = MaxNumOfVariables(file);
        setDependentVariable();
        InitializeDataPoints(file);

        file.close();
    }
    int MaxNumOfVariables(ifstream & file){
        if(!file.is_open()){
            cout << "File not found" << endl;
            exit(1);
        }
        string temp;
        string Var = "";
        int max = 0;
        getline(file,temp);
        for(int i = 0 ; i <= temp.length()+1 ; i++){
            if(temp[i] == separator || (i == temp.length()+1)){
                max++;
                NameOfAllVariables.push_back(Var);
                Var = "";
            }else{
                Var += temp[i];
            }
        }
        return max;
    }

    bool areAllFieldsCorrect(string temp) {
        char prev = '\0'; 
        char c;
        int count = 0;
        for (int i = 0; i < temp.size(); i++) {
            c = temp[i];

            if (c == separator && prev == separator) {
                return false;
            }
            if (!isCharValid(c) && c != separator) {
                return false; 
            }
            prev = c;
            if(c == separator){
                count++;
            }
        }
        if ((count+1) != MaxVariablesQty){
            return false;
        }
        
        return true;
    }

    bool isCharValid(char c) {
        if ((c >= '0' && c <= '9') || (c == '.')){
            return true;
        }
        return false;
    }
    void setDependentVariable(){
        cout << "\nSelect the appropriate number for the desired Dependent Variable :- " << endl;
        int i;
        for(int i = 0 ; i < MaxVariablesQty ; i++){
            cout << i+1 << ". " << NameOfAllVariables[i] << endl;
        }
        do {
            cout << "Enter the number corresponding to the Dependent Variable : ";
            i = 6;
            cin >> i;
        }while(i < 1 || i > MaxVariablesQty);
        ColumnIndexForDependentVariable = i-1;
    }

    void InitializeDataPoints(ifstream & file){
        if(!file.is_open()){
            cout << "File not found" << endl;
            exit(1);
        }
        string temp;
        file.seekg(0);
        while(!file.eof()){
            getline(file,temp);
            int ColumnIndex = 0;
            vector<double> tempX;
            double tempY;
            
            if (areAllFieldsCorrect(temp)){
               string digits = "";
                for (int i = 0 ; i < temp.size() + 1; i++){

                    if (((temp[i] >= '0' && temp[i] <= '9') || temp[i] == '.')){
                        digits += temp[i];
                    }else{
                        if (temp[i] == separator || i == temp.size()){
                            if (ColumnIndex == ColumnIndexForDependentVariable){
                                tempY = stod(digits);
                            }else{
                                tempX.push_back(stod(digits));
                            }
                            digits = "";
                            ColumnIndex++;
                        }
                    }
                }
                DP.push_back(DataPoint(tempX,tempY));
                NumberOfRows++;
            }
        }
        NumberOfRows + 1;
    }


    void InitializeTrainingData(double Percentage){
        if (Percentage < 0 || Percentage > 1){
            cout << "Percentage must be between 0 and 1" << endl;
            cout << "Re-enter Percentage : ";
            cin >> Percentage;
            return InitializeTrainingData(Percentage);
        }else{    
            Training.clear();
            Testing.clear();
            double NumOfLines = NumberOfRows * Percentage;

            for (int i = 0 ; i < NumberOfRows ; i++){
                if (i < NumOfLines){
                    Training.push_back(DP[i]);
                }else{
                    Testing.push_back(DP[i]);
                }
            }
        }
    }
    vector<DataPoint> getTrainingDataPoints(){
        return Training;
    }
    vector<DataPoint> getTestingDataPoints(){
        return Testing;
    }


    void DisplayData(){
        cout << "\n\nAll Variables :-\n";
        for(int i = 0 ; i < NameOfAllVariables.size() ; i++){
            cout << NameOfAllVariables[i] << ", ";
        }
        cout << "\n\nDependent variable : " << NameOfAllVariables[ColumnIndexForDependentVariable] << endl;

    cout << "\nAll data Points :- " << endl;
    for(int i = 0; i < DP.size(); i++) {
        cout << i+1 << ". ";
        for(int j = 0; j < DP[i].x.size(); j++) {
            cout << DP[i].x[j] << ", ";
        }
        cout << " | " << DP[i].getY();
        cout << endl;
    }
        
    }
};

int main() {
    Data data("Data.csv");
    data.DisplayData();

    double TrainingPer = 0.75;
    data.InitializeTrainingData(TrainingPer); //first call this 

    cout << "\n\nTraining data :- " << endl;
    vector<DataPoint> ForTraining = data.getTrainingDataPoints(); // then this
    for(int i = 0 ; i < ForTraining.size() ; i++){
        ForTraining[i].Display();
    }
    
    // ForTesting[numOfDataPoint].getX(WhichXValue);
    // ForTesting[numOfDataPoint].getY();

    cout << "\n\nTesting data :- " << endl;
    vector<DataPoint> ForTesting = data.getTestingDataPoints(); // or this
    for(int i = 0 ; i < ForTesting.size() ; i++){
        ForTesting[i].Display();
    }
    return 0;
}