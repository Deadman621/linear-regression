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
    vector<double> data;
    

    int NumOfIndependents;
    int MaxVariables;
    public:
    Data(string Name, int n){
        fileName = Name;
        ifstream file(fileName, ios::in);
        
        MaxVariables = MaxNumOfVariables(file);
        setNumOfIndependents(n);
        InitializeData(file);

        file.close();
    }
    int MaxNumOfVariables(ifstream & file){
        if(!file.is_open()){
            cout << "File not found" << endl;
            exit(1);
        }
        string temp;
        int max = 0;
        while(!file.eof()){
            getline(file,temp);

            //cout << "temp : " << temp << endl;
            
            if (areAllFieldsCorrect(temp)){
               int count = 0;
                for (int i = 0 ; i < temp.size(); i++){
                    if (temp[i] == ','){
                        count++;
                    }
                    if (count > max){
                        max = count;
                    }  
                }
            }
        }
        return max + 1;
    }

    bool areAllFieldsCorrect(string temp) {
        char prev = '\0'; 
        for (int i = 0; i < temp.size(); i++) {
            char c = temp[i];

            if (c == ',' && prev == ',') {
                return false;
            }
            if (!isCharValid(c) && c != ',') {
                return false; 
            }
            prev = c;
        }
        return true;
    }

    bool isCharValid(char c) {
        if ((c >= '0' && c <= '9') || (c == '.')){
            return true;
        }
        return false;
    }
    void setNumOfIndependents(int n){
        while(n >= MaxVariables){
            cout << "\nNumber of Independents can't exceed or be equals to the total number of variables." << endl;
            cout << "Enter the number of Independents : ";
            cin >> n;
        }
        NumOfIndependents = n;
    }

    void InitializeData(ifstream & file){
        if(!file.is_open()){
            cout << "File not found" << endl;
            exit(1);
        }
        string temp;
        int max = 0;
        file.seekg(0);
        while(!file.eof()){
            getline(file,temp);
            if (areAllFieldsCorrect(temp)){
               string parts = "";
                for (int i = 0 ; i <= temp.size(); i++){
                    
                    cout << temp[i] << endl;
                    
                    if (((temp[i] >= '0' && temp[i] <= '9') || temp[i] == '.')){
                        parts += temp[i];
                    }else{
                        if (parts != ""){
                            data.push_back(stod(parts));
                            parts = "";
                        }
                    }
                }
            }
        }
    }

    void DisplayDataAndClassAttributes(){
        cout << "Data : " << endl;
        for (int i = 0 ; i < data.size(); i++){
            cout << data[i] << " ";
        }
        cout << "\n\nNumber of Independents : " << NumOfIndependents << endl;
        cout << "Number of Variables : " << MaxVariables << endl;
    }
};

int main() {
    Data data("data.csv", 1);
    data.DisplayDataAndClassAttributes();
    return 0;
}