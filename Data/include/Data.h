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

    template<typename T> 
    T NormalizeX(const T& x) const {
        if constexpr (std::is_same_v<T, double>) 
            return (x - MinX) / (MaxX - MinX);
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = x;
            for (double& val : temp) {
                val = NormalizeX(val);
            }

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
    T NormalizeY(const T& y) const {
        if constexpr (std::is_same_v<T, double>) 
            return (y - MinY) / (MaxY - MinY);
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = y;
            for (double& val : temp) 
                val = NormalizeY(val);

            return temp;

        } else if constexpr (std::is_same_v<T, std::vector<std::vector<double>>>) {
            std::vector<std::vector<double>> temp = y;
            for (std::vector<double>& vec : temp) 
                vec = NormalizeY(vec);

            return temp;
        }

        throw std::invalid_argument{"Invalid type"};
    }


    template<typename T>
    T DeNormalizeX(const T& x) const {
        if constexpr (std::is_same_v<T, double>) 
            return x * (MaxX - MinX) + MinX;
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = x;
            for (double& val : temp) 
                val = DeNormalizeX(val);

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
    T DeNormalizeY(const T& y) const {
        if constexpr (std::is_same_v<T, double>) 
            return y * (MaxY - MinY) + MinY;
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = y;
            for (double& val : temp) 
                val = DeNormalizeY(val);

            return temp;

        } else if constexpr (std::is_same_v<T, std::vector<std::vector<double>>>) {
            std::vector<std::vector<double>> temp = y;
            for (std::vector<double>& vec : temp) 
                vec = DeNormalizeY(vec);

            return temp;
        }

        throw std::invalid_argument{"Invalid type"};
    }

    template<typename T>
    T StandardizeX(const T& x) const {
        if constexpr (std::is_same_v<T, double>) 
            return (x - MeanX) / StdX;
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = x;
            for (double& val : temp) 
                val = StandardizeX(val);

            return temp;

        } else if constexpr (std::is_same_v<T, std::vector<std::vector<double>>>) {
            std::vector<std::vector<double>> temp = x;
            for (std::vector<double>& vec : temp) 
                vec = StandardizeX(vec);

            return temp;
        }

        throw std::invalid_argument{"Invalid type"};
    }

    template<typename T>
    T DeStandardizeX(const T& x) const {
        if constexpr (std::is_same_v<T, double>) 
            return x * StdX + MeanX;
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = x;
            for (double& val : temp) 
                val = DeStandardizeX(val);

            return temp;

        } else if constexpr (std::is_same_v<T, std::vector<std::vector<double>>>) {
            std::vector<std::vector<double>> temp = x;
            for (std::vector<double>& vec : temp) 
                vec = DeStandardizeX(vec);

            return temp;
        }

        throw std::invalid_argument{"Invalid type"};
    }

    template<typename T>
    T StandardizeY(const T& y) const {
        if constexpr (std::is_same_v<T, double>) 
            return (y - MeanY) / StdY;
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = y;
            for (double& val : temp) 
                val = StandardizeY(val);

            return temp;

        } else if constexpr (std::is_same_v<T, std::vector<std::vector<double>>>) {
            std::vector<std::vector<double>> temp = y;
            for (std::vector<double>& vec : temp) 
                vec = StandardizeY(vec);

            return temp;
        }

        throw std::invalid_argument{"Invalid type"};
    }

    template<typename T>
    T DeStandardizeY(const T& y) const {
        if constexpr (std::is_same_v<T, double>) 
            return y * StdY + MeanY;
        else if constexpr (std::is_same_v<T, std::vector<double>>) {
            std::vector<double> temp = y;
            for (double& val : temp) 
                val = DeStandardizeY(val);

            return temp;

        } else if constexpr (std::is_same_v<T, std::vector<std::vector<double>>>) {
            std::vector<std::vector<double>> temp = y;
            for (std::vector<double>& vec : temp) 
                vec = DeStandardizeY(vec);

            return temp;
        }

        throw std::invalid_argument{"Invalid type"};
    }
};

#endif