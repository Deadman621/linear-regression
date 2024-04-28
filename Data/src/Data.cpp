#include <Data.h>

using namespace std;

// Implementations of File class
File::File(string n, string t){
    fileName = n;
    Type = t;
}

File::~File() {
    if(file.is_open()){
        file.close();
    }
}

// Implementations of CSVFile class
CSVFile::CSVFile(string n) : File(n, "CSV"), separator(',') {}

// Implementations of DataPoint class
DataPoint::DataPoint(){}

DataPoint::DataPoint(vector<double> X, double Y){
    x = X;
    y = Y;
}

vector<double> DataPoint::getX() const{
    return x;
}

double DataPoint::getY() const{
    return y;
}

double DataPoint::getX(int i) const{
    return x[i];
}

void DataPoint::Display() const{
    cout << x[0];
    for (int i = 0; i < x.size(); i++) {
        cout << ", " << x[i];
    }
    cout << " | ";
    cout << y << endl;
}

// Implementations of Data class
Data::Data(string Name) : CSVFile(Name) {
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

Data::Data(const Data& d) 
    : CSVFile{d.fileName}, MaxVariablesQty{d.MaxVariablesQty}, 
      NameOfAllVariables{d.NameOfAllVariables}, 
      ColumnIndexForDependentVariable{d.ColumnIndexForDependentVariable}, 
      DP{d.DP}, Training{d.Training}, Testing{d.Testing}, NumberOfRows{d.NumberOfRows} 
    {
        file.open(fileName);
        if(!file.is_open()){
            cout << "File not found" << endl;
            exit(1);
        }

        file.seekg(const_cast<ifstream&>(d.file).tellg());
    }

int Data::MaxNumOfVariables(ifstream & file) {
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

bool Data::areAllFieldsCorrect(string temp) {
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

bool Data::isCharValid(char c) {
    if ((c >= '0' && c <= '9') || (c == '.')){
        return true;
    }
    return false;
}

void Data::setDependentVariable() {
    cout << endl << "Select the appropriate number for the desired Dependent Variable: " << endl;
    int i;
    for(int i = 0 ; i < MaxVariablesQty ; i++){
        cout << i+1 << ". " << NameOfAllVariables[i] << endl;
    }
    do {
        cout << "Enter the number corresponding to the Dependent Variable: ";
        i = 6;
        cin >> i;
    }while(i < 1 || i > MaxVariablesQty);
    ColumnIndexForDependentVariable = i-1;
}

void Data::InitializeDataPoints(ifstream & file) {
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

void Data::InitializeTrainingData(double Percentage) {
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

vector<DataPoint> Data::getTrainingDataPoints() const {
    return Training;
}

vector<DataPoint> Data::getTestingDataPoints() const {
    return Testing;
}

vector<vector <double>> Data::getTestingX() const {
    vector<vector<double>> X;
    for (int i = 0; i < Testing.size(); i++) {
        X.push_back(Testing[i].getX());
    }
    return X;   
}

vector<double> Data::getTestingY() const {
    vector<double> Y;
    for (int i = 0; i < Testing.size(); i++) {
        Y.push_back(Testing[i].getY());
    }
    return Y;
}

vector<vector<double>> Data::getTrainingX() const {
    vector<vector<double>> X;
    for (int i = 0; i < Training.size(); i++) {
        X.push_back(Training[i].getX());
    }
    return X;
}

vector<double> Data::getTrainingY() const {
    vector<double> Y;
    for (int i = 0; i < Training.size(); i++) {
        Y.push_back(Training[i].getY());
    }
    return Y;
}

void Data::DisplayData() const {
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