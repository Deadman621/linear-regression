#include<cmath>
#include<tuple>
#include<iostream>
#include<stdexcept>
#include<regression.h>
#include<HyperParameterOptimization.h>
//#include<matplot/matplot.h>

using namespace std;

void checkNaN(double value, const string& errorMessage) {
    if (isnan(value)) 
        throw invalid_argument{errorMessage};
}

double RegressionEquation(const Model& model, size_t data_point) {
    double y = 0;
    for(size_t i = 0; i < model.numFeatures; i++) 
        y += model.m[i] * model.x.at(data_point)[i];
    return y + model.b;
}

double RegressionEquation(const Model& model, const vector<double>& x) {
    if (x.size() == 0) 
        throw invalid_argument{"Empty dataset"};
    if (model.m.size() == 0)
        throw invalid_argument{"Empty weights"};
    double y = 0;
    for(size_t i = 0; i < x.size(); i++) 
        y += model.m[i] * x[i];
    return y + model.b;
}

Model::Model(Data& d) 
    : data{d}, b{0}, error{numeric_limits<double>::max()}, 
        learning_rate{0.0001},
        numBatches{0}
{
    tie(this->x, this->y) = data.getTrainingData(true, false);

    if (y.empty()) 
        throw runtime_error("Y dataset is empty");

    for(auto const& data_point : x) 
        if (x.empty())
            throw runtime_error{"X datasets are empty"};

    this->numFeatures = x[0].size();
    this->numDataPoints = y.size();

    m = vector<double>(numFeatures, 0.0);
 
}

void Model::Optimize(void) {
    try {
        Data evalData = this->data.GetEvalData();    
        evalData.InitializeTrainingData(0.8);

        if (this->optimizer == nullptr) {
            this->optimizer = new HyperParameteroptimization{evalData};
            if (this->optimizer == nullptr)
                throw runtime_error{"Failed to initialize optimizer"};

            if (this->optimizer->model_ == nullptr) {
                Model* model = new Model{evalData};
                if (model == nullptr)
                    throw runtime_error{"Failed to initialize model"};
                this->optimizer->setModel(model);
            }
        }

        vector<double> LearningRate_Values{0.0001, 0.001, 0.01, 0.1};
        vector<double> epochs_values{10, 50, 70, 100, 200, 300, 400, 500, 1000};

/*         for(size_t i = 0; i < 5; i++) {
            int epochs = numDataPoints / (100 * (i + 1));
            epochs = max(epochs, 1); 
            epochs_values.push_back(epochs);
        } */

        double GridSearch = optimizer->GridSearch(LearningRate_Values, epochs_values, this->optimizer->dataset_.getTrainingDataPoints(true, false));
        double RandomSearch = optimizer->RandomSearch(LearningRate_Values, epochs_values, this->optimizer->dataset_.getTrainingDataPoints(true, false));

/*         cout << "Best Learning Rate: " << optimizer->best_learningrate << endl;
        cout << "Best Epochs: " << optimizer->best_epochs << endl;
        cout << "Best Batch Size: " << optimizer->bestBatchSize << endl; */

        this->learning_rate = optimizer->best_learningrate;

    } catch (const runtime_error& e) {
        delete this->optimizer;
        this->optimizer = nullptr;
        throw e;
    } catch (const exception& e) {
        delete this->optimizer;
        this->optimizer = nullptr;
        throw e;
    } catch (...) {
        cerr << "Unknown exception caught" << endl;
        delete this->optimizer;
        this->optimizer = nullptr;
        exit(1);
    }
}

void Model::SetLearningRate(double rate) { this->learning_rate = rate; }

double Model::GetLearningRate(void) const noexcept { return this->learning_rate; }

double Model::MeanSquaredError(size_t start_index, size_t batch_size) { 
    this->error = 0;
    size_t end_index = min(start_index + batch_size, this->y.size());
    size_t n = end_index - start_index;
    for (size_t i = start_index; i < end_index; i++) 
        this->error += pow((this->y[i] - RegressionEquation(*this, i)), 2);
    this->error /= n;

    return this->error;
}

// Check Error on Testing Dataset
double Model::MeanSquaredError(const Data& d) const {
    double Error = 0;
    vector<vector<double>> x;
    vector<double> y;
    tie(x, y) = d.getTestingData(true, false);
    for (int i = 0; i < y.size(); i++) 
        Error += pow((y[i] - RegressionEquation(*this, x[i])), 2);
    Error /= y.size();

    return Error;
}

double Model::MeanAbsolutePercentageError(const Data& d) const {
    double Error = 0;
    vector<vector<double>> x;
    vector<double> y;
    tie(x, y) = d.getTestingData(true, false);
    for (int i = 0; i < y.size(); i++) {
        double prediction = RegressionEquation(*this, x[i]);
        Error += abs((y[i] - prediction) / y[i]);
    }
    Error /= y.size();

    return Error * 100;  // Return as percentage
}

void Model::GradientDescent(size_t start_index, size_t batch_size) {
    vector<double> m_gradient(numFeatures, 0.0);
    double b_gradient = 0;
    size_t end_index = min(start_index + batch_size, this->y.size());
    size_t n = end_index - start_index;

    for (size_t i = start_index; i < end_index; i++) {
        double prediction = RegressionEquation(*this, i);
        for(size_t j = 0; j < numFeatures; j++) 
            m_gradient[j] += -(2.0 / n) * x[i][j] * (y[i] - prediction);

        b_gradient += -(2.0 / n) * (y[i] - prediction);
    }

    for(size_t i = 0; i < m.size(); i++) 
        m[i] -= m_gradient[i] * learning_rate;
    b -= b_gradient * learning_rate;
}

void Model::Train(size_t epochs, size_t batch_size, bool display_batch) {
    
    if (epochs == 0)
        throw invalid_argument{"Epochs cannot be zero (Train)"};
    
    if (batch_size == 0)
        throw invalid_argument{"Batch size cannot be zero (Train)"};

    for (size_t epoch = 0; epoch < epochs; epoch++) {
        for (size_t i = 0; i < numDataPoints; i += batch_size) {
            size_t current_batch_size = min(batch_size, numDataPoints - i);
            GradientDescent(i, current_batch_size);
            MeanSquaredError(i, current_batch_size);
        }

        if (epoch % 500 == 0 && display_batch) 
            cout << "Epoch: " << epoch << " MSE: " << error << endl;
    }
}

void Model::Train(bool display_batch) {
    
    Optimize();

    size_t epochs = optimizer->best_epochs;
    size_t batch_size = optimizer->bestBatchSize;

    Model::Train(epochs, batch_size, display_batch);
}

void Model::DisplayPlot(void) {
/*     vector<double> x0, x1, dx0, dx1, dy;

    for (const auto& data_point : x) {
        x0.push_back(data_point[0]);
        dx0.push_back(data.DeNormalizeX(data_point[0], 0));
        if (numFeatures == 2) {
            x1.push_back(data_point[1]);
            dx1.push_back(data.DeNormalizeX(data_point[1], 1));
        }
    }

    dy = data.DeNormalizeY(y);

    if (numFeatures == 1) {
        vector<double> x_line = matplot::linspace(matplot::min(dx0), matplot::max(dx0), numDataPoints);
        vector<double> y_line = matplot::transform(x_line, [this](auto x_line) { return m[0] * data.NormalizeX(x_line, 0) + b; });
        y_line = data.DeNormalizeY(y_line);

        matplot::figure();
        matplot::hold(matplot::on);
        matplot::scatter(dx0, dy)->color("black").marker(".").fill(true).marker_size(10);
        matplot::plot(x_line, y_line)->color("black").line_width(2.0);
        matplot::show();
        cin.get();

    } else if (numFeatures == 2) {
        vector<double> x_line = matplot::linspace(matplot::min(dx0), matplot::max(dx0), numDataPoints);
        vector<double> y_line = matplot::linspace(matplot::min(dx1), matplot::max(dx1), numDataPoints);
        vector<double> z_line = matplot::transform(x_line, y_line, [this](double x, double y) { return m[0] * data.NormalizeX(x, 0) + m[1] * data.NormalizeX(y, 1) + b; });
        z_line = data.DeNormalizeY(z_line);

        matplot::figure();
        matplot::scatter3(dx0, dx1, dy)->color("black").marker(".").fill(true).marker_size(10);
        matplot::hold(matplot::on);
        matplot::plot3(x_line, y_line, z_line)->color("black").line_width(2.0);
        matplot::view(-30, 1);
        matplot::show();
        cin.get();
    } */
}

vector<double> Model::Predict(const Data& d) const {
    vector<double> predictions;
    vector<vector<double>> test = d.getTestingData(true, false).first;
    for (size_t i = 0; i < test.size(); i++) 
        predictions.push_back(RegressionEquation(*this, test[i]));

    return this->data.DeNormalizeY(predictions);
}

ostream& operator<<(ostream& output, const Model& model) {
    output << endl << "Weights (Normalized): " << endl;
    for(size_t i = 0; i < model.m.size(); i++) 
        output << "m" << i << ": " << model.m[i] << endl;
    output << "Bias: " << model.b << endl;

    return output;
}

Model::Model(const Model& model) 
    : data{model.data}, b{model.b}, error{model.error}, 
      numFeatures{model.numFeatures}, numDataPoints{model.numDataPoints}, 
      numBatches{model.numBatches}, m{model.m}, x{model.x}, y{model.y}, 
      learning_rate{model.learning_rate} {}

bool Model::operator==(const Model& model) const {
    return { 
        (this->m == model.m) && 
        (this->b == model.b) && 
        (this->x == model.x) && 
        (this->y == model.y) && 
        (this->data == model.data) &&
        (this->numFeatures == model.numFeatures) &&
        (this->numDataPoints == model.numDataPoints) &&
        (this->error == model.error) &&
        (this->learning_rate == model.learning_rate)
    }; 
} 

Model& Model::operator=(const Model& model) {
    if (this == &model) 
        return *this;

    this->m = model.m;
    this->b = model.b;
    this->x = model.x;
    this->y = model.y;
    this->data = model.data;
    this->numFeatures = model.numFeatures;
    this->numDataPoints = model.numDataPoints;
    this->error = model.error;
    this->learning_rate = model.learning_rate;

    return *this;
}

vector<double> Model::operator()(const vector<vector<double>>& x) const {
    vector<double> predictions;
    for (size_t i = 0; i < x.size(); i++) 
        predictions.push_back(RegressionEquation(*this, x[i]));

    return this->data.DeNormalizeY(predictions);
}

vector<double> Model::operator()(const Data& d) const {
    return Predict(d);
}

Model::~Model(void) {
    if (this->optimizer != nullptr) 
        delete this->optimizer;
}