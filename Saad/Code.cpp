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

    vector<vector<double>> x;
    vector<double> y;
    
    public:
    DataPoint(){};
    DataPoint(vector<vector<double>> X, vector<double> Y){
        x = X;
        y = Y;
    }
    vector<vector<double>> getX(){
        return x;
    }
    vector<double> getY(){
        return y;
    }
    vector<double> getX(int i){
        return x[i];
    }
    double getY(int i){
        return y[i];
    }
    double getX(int i, int j){
        return x[i][j];
    }
    void Display(){
        for (int i = 0; i < x.size(); i++) {
            cout << x[i][0];
            for (int j = 1; j < x[i].size(); j++) {
                cout << ", " << x[i][j];
            }
        }
        cout << " | ";
        for (int i = 0; i < y.size(); i++) {
            cout << y[i] << " ";
        }
        cout << endl;
    }
    friend class Data;
};

class Data : public CSVFile {
    int MaxVariablesQty;
    vector<string> NameOfAllVariables;
    int ColumnIndexForDependentVariable;

    vector<DataPoint> DP;
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
            vector<double> tempY;
            vector<vector<double>> temp_tempX;
            
            if (areAllFieldsCorrect(temp)){
               string digits = "";
                for (int i = 0 ; i < temp.size() + 1; i++){

                    if (((temp[i] >= '0' && temp[i] <= '9') || temp[i] == '.')){
                        digits += temp[i];
                    }else{
                        if (temp[i] == separator || i == temp.size()){
                            if (ColumnIndex == ColumnIndexForDependentVariable){
                                tempY.push_back(stod(digits));
                            }else{
                                tempX.push_back(stod(digits));
                            }
                            digits = "";
                            ColumnIndex++;
                        }
                    }
                }
                temp_tempX.push_back(tempX);
                DP.push_back(DataPoint(temp_tempX,tempY));
                NumberOfRows++;
            }
        }
    }


    vector<vector<DataPoint>> getDividedData(double Percentage){
        if (Percentage < 0 || Percentage > 1){
            cout << "Percentage must be between 0 and 1" << endl;
            cout << "Re-enter Percentage : ";
            cin >> Percentage;
            return getDividedData(Percentage);
        }else{
            vector<DataPoint> Training;
            vector<DataPoint> Testing;
            int NumOfLines = NumberOfRows * Percentage;
            for (int i = 0 ; i < NumberOfRows ; i++){
                if (i < NumOfLines){
                    Training.push_back(DP[i]);
                }else{
                    Testing.push_back(DP[i]);
                }
            }
            vector<vector<DataPoint>> temp;
            temp.push_back(Training);
            temp.push_back(Testing);
            return temp;
        }
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
            cout << DP[i].getX(j,0);
            for(int k = 1; k < DP[i].getX(j).size(); k++) { // or DP[i].x[j].size();
                cout << ", " << DP[i].x[j][k]; // or DP[i].getX(j,k); 
            }
        }
        cout << " | " << DP[i].getY()[0];
        cout << endl;
    }
        
    }
};

int main() {
    Data data("Data.csv");
    data.DisplayData();

    vector<vector<DataPoint>> DividedData = data.getDividedData(0.5);
    cout << "\n\nTraining Data :- \n";
    //Training data stored at index 0
    for(int i = 0; i < DividedData[0].size(); i++) {
        DividedData[0][i].Display();
    }



    int WhichDataPoint = 0; // 0 index = first
    //first index of DividedData is 0 because we want to get the training data
    vector<vector<double>> TrainingX_1 = DividedData[0][WhichDataPoint].getX();
    vector<double> TrainingY_1 = DividedData[0][WhichDataPoint].getY();
    vector<vector<double>> TrainingX_2 = DividedData[0][WhichDataPoint + 1].getX();
    vector<double> TrainingY_2 = DividedData[0][WhichDataPoint + 1].getY();


    cout << "\n\nTraining Data 1 :- \n";
    for(int i = 0; i < TrainingX_1.size(); i++) {
        cout << "x : ";
        for(int j = 0; j < TrainingX_1[i].size(); j++) {
            cout << TrainingX_1[i][j] << " ";
        }
        cout << "| y : " << TrainingY_1[i];
    }
    cout << "\n\nTraining Data 2 :- \n";
    for(int i = 0; i < TrainingX_2.size(); i++) {
        cout << "x : ";
        for(int j = 0; j < TrainingX_2[i].size(); j++) {
            cout << TrainingX_2[i][j] << " ";
        }
        cout << "| y : " << TrainingY_2[i];
    }
    
    //Testing data stored at index 1
    cout << "\n\nTesting Data :- \n";
    for(int i = 0; i < DividedData[1].size(); i++) {
        DividedData[1][i].Display();
    }
    
    
    //first index of DividedData is 1 because we want to get the testing data
    vector<vector<double>> TestingX_1 = DividedData[1][WhichDataPoint].getX();
    vector<double> TestingY_1 = DividedData[1][WhichDataPoint].getY();
    vector<vector<double>> TestingX_2 = DividedData[1][WhichDataPoint + 1].getX();
    vector<double> TestingY_2 = DividedData[1][WhichDataPoint + 1].getY();
    
    cout << "\n\nTesting Data 1 :- \n";
    for(int i = 0; i < TestingX_1.size(); i++) {
        cout << "x : ";
        for(int j = 0; j < TestingX_1[i].size(); j++) {
            cout << TestingX_1[i][j] << " ";
        }
        cout << "| y : " << TestingY_1[i];
    }
    cout << "\n\nTesting Data 2 :- \n";
    for(int i = 0; i < TestingX_2.size(); i++) {
        cout << "x : ";
        for(int j = 0; j < TestingX_2[i].size(); j++) {
            cout << TestingX_2[i][j] << " ";
        }
        cout << "| y : " << TestingY_2[i];
    }

    return 0;
}