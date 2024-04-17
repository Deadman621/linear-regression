#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;


class Data {
    fstream file;
    string fileName;

    vector<vector<double>> x;
    vector<double> y;
    
    int MaxVariablesQty;
    vector<string> NameOfAllVariables;
    int ColumnNumberForDependentVariable;

    public:
    Data(string Name){
        fileName = Name;
        ifstream file(fileName, ios::in);
        
        MaxVariablesQty = MaxNumOfVariables(file);
        setDependentVariable();
        SetData(file);

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
        if ((count+1) > MaxVariablesQty){
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
        for(int i = 0 ; i < MaxVariablesQty ; i++){
            cout << i+1 << ". " << NameOfAllVariables[i] << endl;
        }
        cout << "\nEnter the number corresponding to the Dependent Variable : ";
        int i;
        cin >> i;
        ColumnNumberForDependentVariable = i-1;
    }

    void SetData(ifstream & file){
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
                            if (ColumnIndex == ColumnNumberForDependentVariable){
                                y.push_back(stod(digits));
                            }else{
                                tempX.push_back(stod(digits));
                            }
                            digits = "";
                            ColumnIndex++;
                        }
                    }
                }
                x.push_back(tempX);
            }
        }
    }

    void DisplayDataAndClassAttributes(){
        cout << "\n\nAll Variables :-\n";
        for(int i = 0 ; i < NameOfAllVariables.size() ; i++){
            cout << NameOfAllVariables[i] << ", ";
        }

        cout << "\n\nX :-\n";
        for(int i = 0 ; i < x.size() ; i++){
            for(int j = 0 ; j < x[i].size() ; j++){
                cout << x[i][j] << " ";
            }
            cout << endl;
        }
        cout << "Y :-\n";
        for(int i = 0 ; i < y.size() ; i++){
            cout << y[i] << endl;
        }
        
        cout << "\n\nDependent variable : " << NameOfAllVariables[ColumnNumberForDependentVariable] << endl;
        cout << "Total Number of Variables : " << MaxVariablesQty << endl;
    }
};

int main() {
    Data data("Data.csv");
    data.DisplayDataAndClassAttributes();
    return 0;
}