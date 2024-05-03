#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>
#include <fstream>
#include <algorithm> // for sort (vector)
#include <cmath> // for sqrt
#include <iostream>

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
    virtual void DisplayData() const = 0;
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
    friend class Data;
};

class Data : public CSVFile {
    int MaxVariablesQty;
    std::vector<std::string> NameOfAllVariables;
    int ColumnIndexForDependentVariable;

    std::vector<DataPoint> DP;
    std::vector<DataPoint> Training;
    std::vector<DataPoint> Testing;
    int NumberOfRows;
    
    public:
    Data(std::string Name);
    Data(const Data& d);
    int MaxNumOfVariables(std::ifstream & file);
    bool areAllFieldsCorrect(std::string temp);
    bool isCharValid(char c);
    void setDependentVariable();
    void InitializeDataPoints(std::ifstream & file);
    void InitializeTrainingData(double Percentage);
    std::vector<DataPoint> getTrainingDataPoints() const;
    std::vector<DataPoint> getTestingDataPoints() const;
    std::vector<std::vector <double>> getTestingX() const;
    std::vector<double> getTestingY() const;
    std::vector<std::vector<double>> getTrainingX() const;
    std::vector<double> getTrainingY() const;
    void DisplayData() const override;
};

#endif