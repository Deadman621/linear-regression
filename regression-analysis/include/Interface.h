#ifndef INTERFACE_H
#define INTERFACE_H

#include<Data.h>

#include <string>

template<typename T>
class iSaveable {
    public:
        virtual void Save(const T& model, const std::string& filename) = 0;
        virtual T Load(const std::string& filename) = 0;
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
        virtual double MeanAbsolutePercentageError(const Data& d) const = 0;
        virtual double MeanSquaredError(size_t start_index, size_t batch_size) = 0;
        virtual double MeanSquaredError(const Data& d) const = 0;
    };

class IDisplayable {
    public:
        virtual void DisplayPlot(void) = 0;
    };

#endif 