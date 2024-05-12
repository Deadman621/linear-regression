#include<regression.h>

using namespace std;

void CheckFileState(const ofstream& file) {
    if (!file) 
        throw runtime_error{"Write operation failed."};
}

void CheckFileState(const ifstream& file) {
    if (!file) 
        throw runtime_error{"Read operation failed."};
}

void SaveModel::Save(const Model& model, const string& filename) {
    ofstream file;
    file.exceptions(ofstream::failbit | ofstream::badbit);
    file.open(path + filename + ".bin", ios::binary);
    if (!file.is_open()) 
        throw invalid_argument{"File not found"};

    size_t filenameSize = model.data.fileName.size();
    file.write(reinterpret_cast<const char*>(&filenameSize), sizeof(filenameSize));
    file.write(model.data.fileName.c_str(), filenameSize);
    file.write(reinterpret_cast<const char*>(&model.data.ColumnIndexForDependentVariable), sizeof(model.data.ColumnIndexForDependentVariable));
    file.write(reinterpret_cast<const char*>(&model.data.trainingPercentage), sizeof(model.data.trainingPercentage));
    file.write(reinterpret_cast<const char*>(&model.data.EVALPERCENTAGE), sizeof(model.data.EVALPERCENTAGE));
    file.write(reinterpret_cast<const char*>(&model.b), sizeof(model.b));
    file.write(reinterpret_cast<const char*>(&model.error), sizeof(model.error));

    size_t vectorSize = model.m.size();
    file.write(reinterpret_cast<const char*>(&vectorSize), sizeof(vectorSize));
    for (const double& element : model.m) 
        file.write(reinterpret_cast<const char*>(&element), sizeof(element));

    file.close();
}

Model SaveModel::Load(const string& filename) {
    ifstream file;
    file.exceptions(ifstream::failbit | ifstream::badbit);
    file.open(path + filename + ".bin", ios::binary);
    if (!file.is_open()) 
        throw invalid_argument{"File not found"};

    size_t filenameSize;
    file.read(reinterpret_cast<char*>(&filenameSize), sizeof(filenameSize));

    char* filenameBuffer = new char[filenameSize + 1]; 
    file.read(filenameBuffer, filenameSize);
    filenameBuffer[filenameSize] = '\0';

    int ColumnIndexForDependantVariable;
    double trainingPercentage;
    double evalpercent;

    file.read(reinterpret_cast<char*>(&ColumnIndexForDependantVariable), sizeof(ColumnIndexForDependantVariable));
    file.read(reinterpret_cast<char*>(&trainingPercentage), sizeof(trainingPercentage));
    file.read(reinterpret_cast<char*>(&evalpercent), sizeof(evalpercent));

    Data d{filenameBuffer, ColumnIndexForDependantVariable, trainingPercentage, evalpercent};
    Model model{d};
    delete[] filenameBuffer;

    file.read(reinterpret_cast<char*>(&model.b), sizeof(model.b));
    file.read(reinterpret_cast<char*>(&model.error), sizeof(model.error));

    size_t vectorSize;
    file.read(reinterpret_cast<char*>(&vectorSize), sizeof(vectorSize));
    model.m.resize(vectorSize);
    for (double& element : model.m) 
        file.read(reinterpret_cast<char*>(&element), sizeof(element));

    file.close();

    return model;
}

bool SaveModel::Exists(const string& filename) {
    ifstream file;
    file.open(path + filename + ".bin", ios::binary);
    return file.is_open();
}

void SaveModel::Delete(const string& filename) {
    if (Exists(filename)) 
        remove((path + filename + ".bin").c_str());
    
    else
        throw invalid_argument{"File not found"};
}