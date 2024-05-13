#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>
#include <fstream>
#include <ostream>
#include <array>
#include <iostream>
#include <stdexcept>
#include <type_traits>

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
    double EVALPERCENTAGE;

    std::vector<DataPoint> DP;
    std::vector<DataPoint> Training;
    std::vector<DataPoint> Testing;
     
    std::vector<double> MinX; 
    std::vector<double> MaxX; 
    std::vector<double> MeanX; 
    std::vector<double> StdX; 
    double MinY;
    double MaxY;
    double MeanY;
    double StdY;

    int NumberOfRows;
    Data(std::string Name, int ColumnIndexForDependentVariable, double trainingPercentage, double evalPercentage); 
    
    public:
    Data(std::string Name, double EVALPERCENTAGE);
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
    Data GetEvalData();
    void DispNormalizedData();
    void DispStandardizedData();
    void DispAllData();

    friend std::ostream& operator<<(std::ostream& os, const Data& d);
    friend class SaveModel;

    template<typename T>
    T NormalizeX(const T& x, int index = 0) const {
        if constexpr (std::is_same_v<T, double>) 
            return (x - MinX[index]) / (MaxX[index] - MinX[index]);
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = x;
            for (int i = 0; i < temp.size(); i++) 
                temp[i] = (temp[i] - MinX[i]) / (MaxX[i] - MinX[i]);

            return temp;

        } else if constexpr (std::is_same_v<T, std::vector<std::vector<double>>>) {
            std::vector<std::vector<double>> temp = x;
            for (std::vector<double>& vec : temp) 
                vec = NormalizeX(vec);

            return temp;
        }

        throw std::invalid_argument{"Invalid type"};
    }

    template<typename T>
    T DeNormalizeX(const T& x, int index = 0) const {
        if constexpr (std::is_same_v<T, double>) 
            return x * (MaxX[index] - MinX[index]) + MinX[index];
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = x;
            for (int i = 0; i < temp.size(); i++) 
                temp[i] = temp[i] * (MaxX[i] - MinX[i]) + MinX[i];

            return temp;

        } else if constexpr (std::is_same_v<T, std::vector<std::vector<double>>>) {
            std::vector<std::vector<double>> temp = x;
            for (std::vector<double>& vec : temp) 
                vec = DeNormalizeX(vec);

            return temp;
        }

        throw std::invalid_argument{"Invalid type"};
    }

    template<typename T>
    T NormalizeY(const T& y) const {
        if constexpr (std::is_same_v<T, double>) 
            return (y - MinY) / (MaxY - MinY);
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = y;
            for (double& val : temp) 
                val = NormalizeY(val);

            return temp;

        } 
        throw std::invalid_argument{"Invalid type"};
    }

    template<typename T>
    T DeNormalizeY(const T& y) const {
        if constexpr (std::is_same_v<T, double>) 
            return y * (MaxY - MinY) + MinY;
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = y;
            for (double& val : temp) 
                val = DeNormalizeY(val);

            return temp;
        }

        throw std::invalid_argument{"Invalid type"};
    }

};

#endif