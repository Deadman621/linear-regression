#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>
#include <fstream>
#include <ostream>
#include <array>
#include <iostream>

class DataPoint;
class Data;
class File;
class CSVFile;

class File {
    protected:
    std::ifstream file;
    std::string fileName;
    std::string Type;

    public:
    File(std::string n, std::string t);
    ~File();
};

class CSVFile : public File {
    protected:
    const char separator;

    public:
    CSVFile(std::string n = "");
};

class DataPoint{
    std::vector<double> x;
    double y;
    
    public:
    DataPoint();
    DataPoint(std::vector<double> X, double Y);
    std::vector<double> getX() const;
    double getY() const;
    double getX(int i) const;
    void Display() const;
    bool operator==(const DataPoint& d) const;
    DataPoint& operator=(const DataPoint& d);

    friend class Data;
    friend std::ostream& operator<<(std::ostream& os, const Data& d);
};

class Data : public CSVFile {
    int MaxVariablesQty;
    std::vector<std::string> NameOfAllVariables;
    int ColumnIndexForDependentVariable;
    double trainingPercentage;

    std::vector<DataPoint> DP;
    std::vector<DataPoint> Training;
    std::vector<DataPoint> Testing;
    
    double MinX;
    double MaxX;
    double MinY;
    double MaxY;
    double MeanX;
    double MeanY;
    double StdX;
    double StdY;

    int NumberOfRows;
    Data(std::string Name, int ColumnIndexForDependentVariable, double trainingPercentage = 0.99); 
    
    public:
    Data(std::string Name);
    Data(const Data& d);
    int MaxNumOfVariables(std::ifstream & file);
    bool areAllFieldsCorrect(std::string temp);
    bool isCharValid(char c);
    void setDependentVariable();
    void InitializeDataPoints(std::ifstream & file);
    void InitializeTrainingData(double Percentage);
    std::size_t getNumDataPoints() const;
    std::vector<DataPoint> getTrainingDataPoints(bool WantNormalized = false, bool WantStandardized = false) const;
    std::vector<DataPoint> getTestingDataPoints(bool WantNormalized = false, bool WantStandardized = false) const;
    std::pair<std::vector<std::vector<double>>, std::vector<double>> getTrainingData(bool WantNormalized = false, bool WantStandardized = false) const;
    std::pair<std::vector<std::vector<double>>, std::vector<double>> getTestingData(bool WantNormalized = false, bool WantStandardized = false) const;
    bool operator==(const Data& d) const;
    Data& operator=(const Data& d);
    DataPoint NormalizeDataPoint(const DataPoint d) const;
    DataPoint StandardizeDataPoint(const DataPoint d) const;
    void DispNormalizedData();
    void DispStandardizedData();
    void DispAllData();

    friend std::ostream& operator<<(std::ostream& os, const Data& d);
    friend class SaveModel;
};

#endif