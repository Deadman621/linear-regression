#ifndef INTERFACE_H
#define INTERFACE_H

#include<Data.h>

#include <string>

template<typename T>
class iSaveable {
    public:
        virtual void SaveModel(const T& model, const std::string& filename) = 0;
        virtual void LoadModel(T& model, const std::string& filename) = 0;
        virtual bool Exists(const std::string& filename) = 0;
        virtual void Delete(const std::string& filename) = 0;
        virtual ~iSaveable() = default;
};

class ITrainable {
    public:
        virtual void Train(size_t epochs, size_t batch_size = 50, bool display_batch = false) = 0;
    };

class IEvaluable {
    public:
        virtual double MeanSquaredError(size_t start_index, size_t batch_size) = 0;
        virtual double MeanSquaredError(const Data& d) const = 0;
    };

class IDisplayable {
    public:
        virtual void DisplayPlot(void) = 0;
    };


template<typename T>
class Save: public iSaveable<T> {
    private:
        std::string path;

    public:
        Save(const std::string& path): path{path} {}

        // Name of Folder in Current Workspace
        void SetSaveDestination(const std::string& saveDestination) { this->saveDestination = saveDestination; }

        void SaveModel(const T& model, const std::string& filename) override {
            std::ofstream file(path + filename + ".bin", std::ios::out | std::ios::binary);
            if (!file.is_open()) 
                throw std::invalid_argument("File could not be created.");
            file.write(reinterpret_cast<const char*>(&model), sizeof(model));
            if (!file) 
                throw std::runtime_error("Write operation failed.");
            file.close();
        }

        void LoadModel(T& model, const std::string& filename) override {
            std::ifstream file(path + filename + ".bin", std::ios::in | std::ios::binary);
            if (!file.is_open()) 
                throw std::invalid_argument("File could not be opened.");
            file.read(reinterpret_cast<char*>(&model), sizeof(model));
            if (!file) 
                throw std::runtime_error("Read operation failed.");
            file.close();
        }

        bool Exists(const std::string& filename) override {
            std::ifstream file(path + filename + ".bin");
            return file.good();
        }

        void Delete(const std::string& filename) override {
            if (std::remove((path + filename + ".bin").c_str()) != 0) 
                throw std::runtime_error("Delete operation failed.");
        }
};

#endif 