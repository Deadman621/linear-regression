#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class DataPoint{

    vector<vector<double>> x;
    vector<double> y;
    
    public:
    DataPoint(){};
    friend class Data;
};

class Data {
    ifstream file;
    string fileName;
    int MaxVariablesQty;
    vector<string> NameOfAllVariables;
    int ColumnIndexForDependentVariable;

    DataPoint DP;
    
    public:
    Data(string Name){
        fileName = Name;
        file.open(fileName);
        if(!file.is_open()){
            cout << "File not found" << endl;
            exit(1);
        }
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
            if(temp[i] == ',' || (i == temp.length()+1)){
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

            if (c == ',' && prev == ',') {
                return false;
            }
            if (!isCharValid(c) && c != ',') {
                return false; 
            }
            prev = c;
            if(c == ','){
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
            if (areAllFieldsCorrect(temp)){
               string digits = "";
                for (int i = 0 ; i < temp.size() + 1; i++){

                    if (((temp[i] >= '0' && temp[i] <= '9') || temp[i] == '.')){
                        digits += temp[i];
                    }else{
                        if (temp[i] == ',' || i == temp.size()){
                            //cout << ColumnIndex << " ";
                            if (ColumnIndex == ColumnIndexForDependentVariable){
                                DP.y.push_back(stod(digits));
                            }else{
                                tempX.push_back(stod(digits));
                            }
                            digits = "";
                            ColumnIndex++;
                        }
                    }
                }
                DP.x.push_back(tempX);
            }
        }
    }

    void DisplayData(){
        cout << "\n\nAll Variables :-\n";
        for(int i = 0 ; i < NameOfAllVariables.size() ; i++){
            cout << NameOfAllVariables[i] << ", ";
        }

        cout << "\n\nX :-\n";
        for(int i = 0 ; i < DP.x.size() ; i++){
            for(int j = 0 ; j < DP.x[i].size() ; j++){
                cout << DP.x[i][j] << " ";
            }
            cout << endl;
        }
        cout << "Y :-\n";
        for(int i = 0 ; i < DP.y.size() ; i++){
            cout << DP.y[i] << endl;
        }
        
        cout << "\n\nDependent variable : " << NameOfAllVariables[ColumnIndexForDependentVariable] << endl;
        cout << "Total Number of Variables : " << MaxVariablesQty << endl;
    }
};

int main() {
    Data data("Data.csv");
    data.DisplayData();
    return 0;
}