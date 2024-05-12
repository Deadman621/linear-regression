#include <Data.h>
#include <limits>
#include <Analysis.h>

using namespace std; 

File::File(string n, string t){
    fileName = n;
    Type = t;
}

File::~File() {
    if(file.is_open()){
        file.close();
    }
}

CSVFile::CSVFile(string n) : File(n, "CSV"), separator(',') {}

DataPoint::DataPoint(){}

DataPoint::DataPoint(vector<double> X, double Y){
    if(X.empty()){
        throw invalid_argument("DataPoint cannot be created with empty X vector");
    }
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
    if(i < 0 || i >= x.size()){
        throw invalid_argument("Index out of range");
    }
    return x[i];
}

void DataPoint::Display() const{
    cout << x[0];
    for (int i = 1; i < x.size(); i++) {
        cout << ", " << x[i];
    }
    cout << " | ";
    cout << y << endl;
}

// Implementations of Data class
Data::Data(string Name) : CSVFile(Name) {
    file.open(fileName);
    if(!file.is_open()){
        throw invalid_argument(fileName + ' ' + "not found");
    }
    NumberOfRows = 0;
    this->EVALPERCENTAGE = 0.1;
    MaxVariablesQty = MaxNumOfVariables(file);
    setDependentVariable();
    InitializeDataPoints(file);

    file.close();
}

Data::Data(std::string Name, int ColumnIndexForDependentVariable, double Percentage, double evalpercent) 
    : CSVFile(Name), ColumnIndexForDependentVariable{ColumnIndexForDependentVariable}, trainingPercentage{Percentage}, EVALPERCENTAGE{evalpercent}
    {
    file.open(fileName);
    if(!file.is_open()){
        throw invalid_argument(fileName + ' ' + "not found");
    }
    NumberOfRows = 0;

    MaxVariablesQty = MaxNumOfVariables(file);
    InitializeDataPoints(file);

    file.close();

    InitializeTrainingData(Percentage);
}

Data::Data(const Data& d) 
    : CSVFile{d.fileName}, MaxVariablesQty{d.MaxVariablesQty}, 
      NameOfAllVariables{d.NameOfAllVariables}, 
      ColumnIndexForDependentVariable{d.ColumnIndexForDependentVariable}, 
      trainingPercentage{d.trainingPercentage},
      DP{d.DP}, Training{d.Training}, Testing{d.Testing}, NumberOfRows{d.NumberOfRows},
      MinX{d.MinX}, MaxX{d.MaxX}, MeanX{d.MeanX}, StdX{d.StdX}, 
      MinY{d.MinY}, MaxY{d.MaxY}, MeanY{d.MeanY}, StdY{d.StdY}, EVALPERCENTAGE{d.EVALPERCENTAGE} 
{}

int Data::MaxNumOfVariables(ifstream & file) {
    if(!file.is_open()){
        throw invalid_argument(fileName + ' ' + "not found");
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
        cin >> i;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            i = 0;
        }
    } while(i < 1 || i > MaxVariablesQty);
    ColumnIndexForDependentVariable = i-1;
}

void Data::InitializeDataPoints(ifstream & file) {
    if(!file.is_open()){
       throw invalid_argument("File not found");
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
                        double value = stod(digits);
                    if (ColumnIndex == ColumnIndexForDependentVariable){
                        tempY = value;
                        MinY = min(MinY, value);
                        MaxY = max(MaxY, value);
                    }else{
                        tempX.push_back(value);
                        if (ColumnIndex >= MinX.size()) {
                            MinX.push_back(value);
                            MaxX.push_back(value);
                        } else {
                            MinX[ColumnIndex] = min(MinX[ColumnIndex], value);
                            MaxX[ColumnIndex] = max(MaxX[ColumnIndex], value);
                        }
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
    pair<vector<double>, double> Mean = AnalysisTools::Mean(DP);
    MeanX = Mean.first;
    MeanY = Mean.second;
    pair<vector<double>, double> Std = AnalysisTools::StandardDeviation(DP);
    StdX = Std.first;
    StdY = Std.second;
}

Data Data::GetEvalData(){

    Data temp = Data(*this);

    vector<DataPoint> tempDP;

    for (int i = 0 ; i < NumberOfRows * EVALPERCENTAGE ; i++){
        tempDP.push_back(DP[i]);
    }
    temp.DP.clear();
    temp.Testing.clear();
    temp.Training.clear();
    temp.DP = tempDP;
    temp.NumberOfRows = tempDP.size();
    temp.EVALPERCENTAGE = 0;

/*     cout << endl << tempDP.size();
    cout << endl << temp; */

    return temp;
}
void Data::InitializeTrainingData(double Percentage) {
    
    if (Percentage < 0 || Percentage > 1){
        cout << "Percentage must be between 0 and 1" << endl;
        cout << "Re-enter Percentage : ";
        cin >> Percentage;
        return InitializeTrainingData(Percentage);
    }else{    
        trainingPercentage = Percentage;
        Training.clear();
        Testing.clear();
        double NumOfLines = NumberOfRows * Percentage;
        double NumOfLines2Skip = NumberOfRows * EVALPERCENTAGE;

        for (int i = NumOfLines2Skip ; i < NumberOfRows ; i++){
            if (i < NumOfLines){
                Training.push_back(DP[i]);
            }else{ 
                Testing.push_back(DP[i]);
            }
        }
    }

}

size_t Data::getNumDataPoints() const {
    return NumberOfRows; 
}

vector<DataPoint> Data::getTrainingDataPoints(bool WantNormalized, bool WantStandardized) const {
    if (WantNormalized && WantStandardized) {
        throw invalid_argument("Cannot Normalize and Standardize at the same time");
    }

    if (WantNormalized) {
        vector<DataPoint> temp;
        for (int i = 0; i < Training.size(); i++) {
            temp.push_back(NormalizeDataPoint(Training[i]));
        }
        return temp;
    } else if (WantStandardized) {
        vector<DataPoint> temp;
        for (int i = 0; i < Training.size(); i++) {
            temp.push_back(StandardizeDataPoint(Training[i]));
        }
        return temp;
    }
    for(int i = 0 ; i < Training.size() ; i++){
        Training[i].Display();
    }
    return Training;
}

vector<DataPoint> Data::getTestingDataPoints(bool WantNormalized, bool WantStandardized) const {
    if (WantNormalized && WantStandardized) {
        throw invalid_argument("Cannot Normalize and Standardize at the same time");
    }
    if (WantNormalized) {
        vector<DataPoint> temp;
        for (int i = 0; i < Testing.size(); i++) {
            temp.push_back(NormalizeDataPoint(Testing[i]));
        }
        return temp;
    } else if (WantStandardized) {
        vector<DataPoint> temp;
        for (int i = 0; i < Testing.size(); i++) {
            temp.push_back(StandardizeDataPoint(Testing[i]));
        }
        return temp;
    }
    // for(int i = 0 ; i < Testing.size() ; i++){
    //     Testing[i].Display();
    // }
    return Testing;
}

ostream& operator<<(ostream& os, const Data& data) {
    os << "\n\nAll Variables :-\n";
    for(int i = 0 ; i < data.NameOfAllVariables.size() ; i++){
        os << data.NameOfAllVariables[i] << ", ";
    }
    os << "\n\nDependent variable : " << data.NameOfAllVariables[data.ColumnIndexForDependentVariable] << endl;

    os << "\nAll data Points :- " << endl;
    for(int i = 0; i < data.DP.size(); i++) {
        os << i+1 << ". ";
        for(int j = 0; j < data.DP[i].x.size(); j++) {
            os << data.DP[i].x[j] << ", ";
        }
        os << " | " << data.DP[i].getY();
        os << endl;
    }
    os << "\nTraining Data :- " << endl;
    for(int i = 0; i < data.Training.size(); i++) {
        os << i+1 << ". ";
        for(int j = 0; j < data.Training[i].x.size(); j++) {
            os << data.Training[i].x[j] << ", ";
        }
        os << " | " << data.Training[i].getY();
        os << endl;
    }
    os << "\nTesting Data :- " << endl;
    for(int i = 0; i < data.Testing.size(); i++) {
        os << i+1 << ". ";
        for(int j = 0; j < data.Testing[i].x.size(); j++) {
            os << data.Testing[i].x[j] << ", ";
        }
        os << " | " << data.Testing[i].getY();
        os << endl;
    }
    return os;
}

DataPoint Data::NormalizeDataPoint(const DataPoint d) const {
    vector<double> tempX = d.getX();
    double tempY = d.getY();
    for (int i = 0; i < tempX.size(); i++) {
        tempX[i] = (tempX[i] - MinX[i]) / (MaxX[i] - MinX[i]);
    }
    tempY = (tempY - MinY) / (MaxY - MinY);
    DataPoint temp = DataPoint(tempX, tempY);
    return temp;
}

DataPoint Data::StandardizeDataPoint(const DataPoint d) const {
    vector<double> tempX = d.getX();
    double tempY = d.getY();
    for (int i = 0; i < tempX.size(); i++) {
        tempX[i] = (tempX[i] - MeanX[i]) / StdX[i];
    }

    tempY = (tempY - MeanY) / StdY;
    DataPoint temp(tempX, tempY);
    return temp;
}

void Data::DispNormalizedData(){
    cout << "\nAll Normalized Data :- " << endl;
    for (int i = 0; i < DP.size(); i++) {
        DataPoint temp = NormalizeDataPoint(DP[i]);
        temp.Display();
    }
}

void Data::DispStandardizedData(){
    cout << "\nAll Standardized Data :- " << endl;
    for (int i = 0; i < DP.size(); i++) {
        DataPoint temp = StandardizeDataPoint(DP[i]);
        temp.Display();
    }
}

void Data::DispAllData(){
    cout << "\nAll Data :- " << endl;
    for (int i = 0; i < DP.size(); i++) {
        DP[i].Display();
    }
}

pair<vector<vector<double>>, vector<double>> Data::getTrainingData(bool WantNormalized, bool WantStandardized) const {
    if (WantNormalized && WantStandardized) {
        throw invalid_argument("Cannot Normalize and Standardize at the same time");
    }

    vector<vector<double>> X;
    vector<double> Y;
    for (int i = 0; i < Training.size(); i++) {
        if (WantNormalized) {
            DataPoint temp = NormalizeDataPoint(Training[i]);
            X.push_back(temp.getX());
            Y.push_back(temp.getY());
        } else if (WantStandardized) {
            DataPoint temp = StandardizeDataPoint(Training[i]);
            X.push_back(temp.getX());
            Y.push_back(temp.getY());
        } else {
            X.push_back(Training[i].getX());
            Y.push_back(Training[i].getY());
        }
    }

    return make_pair(X, Y);
}

pair<vector<vector<double>>, vector<double>> Data::getTestingData(bool WantNormalized, bool WantStandardized) const {
    if (WantNormalized && WantStandardized) {
        throw invalid_argument("Cannot Normalize and Standardize at the same time");
    }
    vector<vector<double>> X;
    vector<double> Y;
    DataPoint temp;
    for (int i = 0; i < Testing.size(); i++) {
        if (WantNormalized) {
            temp = NormalizeDataPoint(Testing[i]);
            X.push_back(temp.getX());
            Y.push_back(temp.getY());
        } else if (WantStandardized) {
            temp = StandardizeDataPoint(Testing[i]);
            X.push_back(temp.getX());
            Y.push_back(temp.getY());
        } else {
            X.push_back(Testing[i].getX());
            Y.push_back(Testing[i].getY());
        }
    }
    return make_pair(X, Y);
}

bool DataPoint::operator==(const DataPoint& dp) const {
    return x == dp.x && y == dp.y;
}

DataPoint& DataPoint::operator=(const DataPoint& dp) {
    if (this == &dp) {
        return *this;
    }
    x = dp.x;
    y = dp.y;
    return *this;
}

bool Data::operator==(const Data& d) const {
    return (fileName == d.fileName && Type == d.Type && 
            MaxVariablesQty == d.MaxVariablesQty && 
            NameOfAllVariables == d.NameOfAllVariables && 
            ColumnIndexForDependentVariable == d.ColumnIndexForDependentVariable && 
            DP == d.DP && Training == d.Training && Testing == d.Testing && 
            NumberOfRows == d.NumberOfRows);
}

Data& Data::operator=(const Data& d) {
    if (this == &d) {
        return *this;
    }
    fileName = d.fileName;
    Type = d.Type;
    MaxVariablesQty = d.MaxVariablesQty;
    NameOfAllVariables = d.NameOfAllVariables;
    ColumnIndexForDependentVariable = d.ColumnIndexForDependentVariable;
    DP = d.DP;
    Training = d.Training;
    Testing = d.Testing;
    NumberOfRows = d.NumberOfRows;
    return *this;
}