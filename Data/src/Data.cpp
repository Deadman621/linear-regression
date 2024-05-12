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
        throw invalid_argument("File not found");
    }
    NumberOfRows = 0;
    MaxVariablesQty = MaxNumOfVariables(file);
    setDependentVariable();
    InitializeDataPoints(file);

    file.close();
}

Data::Data(std::string Name, int ColumnIndexForDependentVariable, double Percentage) 
    : CSVFile(Name), ColumnIndexForDependentVariable{ColumnIndexForDependentVariable} 
    {
        file.open(fileName);
        if(!file.is_open()){
            throw invalid_argument("File not found");
        }
        NumberOfRows = 0;
        MinX = numeric_limits<double>::max();
        MaxX = numeric_limits<double>::min();
        MinY = numeric_limits<double>::max();
        MaxY = numeric_limits<double>::min();

        MaxVariablesQty = MaxNumOfVariables(file);
        InitializeDataPoints(file);

        file.close();

        InitializeTrainingData(Percentage);
    }

Data::Data(const Data& d) 
    : CSVFile{d.fileName}, MaxVariablesQty{d.MaxVariablesQty}, 
      NameOfAllVariables{d.NameOfAllVariables}, 
      ColumnIndexForDependentVariable{d.ColumnIndexForDependentVariable}, 
      DP{d.DP}, Training{d.Training}, Testing{d.Testing}, NumberOfRows{d.NumberOfRows} 
    {
        file.open(fileName);
        if(!file.is_open()){
            throw invalid_argument("File not found");
        }
        MinX = numeric_limits<double>::max();
        MaxX = numeric_limits<double>::min();
        MinY = numeric_limits<double>::max();
        MaxY = numeric_limits<double>::min();

        file.seekg(const_cast<ifstream&>(d.file).tellg());
    }

int Data::MaxNumOfVariables(ifstream & file) {
    if(!file.is_open()){
        throw invalid_argument("File not found");
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
    pair<double, double> Mean = AnalysisTools::Mean(DP);
    MeanX = Mean.first;
    MeanY = Mean.second;
    pair<double, double> Std = AnalysisTools::StandardDeviation(DP);
    StdX = Std.first;
    StdY = Std.second;
    pair<double, double> min = AnalysisTools::Min(DP);
    MinX = min.first;
    MinY = min.second;
    pair<double, double> max = AnalysisTools::Max(DP);
    MaxX = max.first;
    MaxY = max.second;
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

        for (int i = 0 ; i < NumberOfRows ; i++){
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
    // for(int i = 0 ; i < Training.size() ; i++){
    //     Training[i].Display();
    // }
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
    return os;
}

DataPoint Data::NormalizeDataPoint(const DataPoint d) const {
    vector<double> tempX = d.getX();
    double tempY = d.getY();
    for (int i = 0; i < tempX.size(); i++) {
        tempX[i] = (tempX[i] - MinX) / (MaxX - MinX);
    }
    tempY = (tempY - MinY) / (MaxY - MinY);
    DataPoint temp = DataPoint(tempX, tempY);
    return temp;
}
DataPoint Data::StandardizeDataPoint(const DataPoint d) const { //useful when there are alot of outliers 
    vector<double> tempX = d.getX();
    double tempY = d.getY();
    for (int i = 0; i < tempX.size(); i++) {
        tempX[i] = (tempX[i] - MeanX) / StdX;
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