## linear-regression

Implementing Linear Regression for Predictive Analysis using CMake for building and managing the project.

### Getting Started with CMake
#### Prerequisites
- CMake version 3.15 or higher.
- A modern C++ compiler that supports C++17 or later.
- Matplot++ library is already added as a subdirectory. You don't need to install manually. Installation instructions can be found at [Matplot++ GitHub repository](https://github.com/alandefreitas/matplotplusplus) just in case.
- Gnuplot for rendering plots generated by Matplot++. Ensure Gnuplot is installed and accessible in your system's PATH. Installation guidelines are available on the [Gnuplot website](http://www.gnuplot.info/).

#### Building the Project
1. Navigate to the project root directory.
2. Create a build directory.
3. Run CMake to configure the project.
4. Build the project.
    - This will compile the project and generate an executable in the build directory.

## Training the Linear Regression Model

This section outlines the steps to train the linear regression model using the provided C++ code. The process involves preparing the dataset, configuring the model, training, and evaluating its performance.

### Dataset Preparation
1. Place your dataset in the `Datasets` directory within the project. The code expects a CSV file format as `[filename].csv`.
2. The dataset should be structured with independent variables (features) in one or more columns and the dependent variable (target) in the last column.

### Model Configuration and Training
1. **Set the Learning Rate**: The learning rate is a crucial hyperparameter that affects how the model learns. In the provided code, it's set to `0.0001` using `model.SetLearningRate(0.0001);`.
2. **Train the Model**: The model is trained with the `model.Train(true);` command, which also outputs training progress if the argument is `true`.
3. **Model Evaluation**: After training, the code evaluates the model's performance by calculating and displaying the standard deviation, mean, and median of the training data's outcomes. It also calculates the mean absolute percentage error (MAPE) on the testing data to assess the model's accuracy.
4. You can always use `main.cpp` as a guide.

### Saving the Trained Model
- The trained model is saved in the `Trained Models` directory with the name `Student_Performance` using the `SaveModel` class.

### Running the Training Process
To run the training process, follow these steps:
1. Navigate to the project root directory.
2. Build the project using CMake:
    ```bash
    mkdir build && cd build
    cmake ..
    cmake --build .
    ```
3. Execute the generated executable, which will train the model and save it to the specified directory.

### Hyperparameter Optimization
Consider experimenting with different learning rates and epochs by manually setting arguments in the `Train(size_t epochs, size_t batch_size, bool display_batch = false)`. If you leave the arguments empty, the program will automatically search the best suitable hyperparameters through grid search and random search (speed and accuracy may be affected).

### Additional Notes
- Ensure CMakeLists.txt is properly set up to include all necessary source files and libraries.
- For detailed instructions on using CMake with your project, refer to the [CMake documentation](https://cmake.org/documentation/).
- For more information on Matplot++ usage and examples, visit the [Matplot++ documentation](https://alandefreitas.github.io/matplotplusplus/).
- Ensure Gnuplot is correctly installed and configured by following the instructions on the [Gnuplot documentation](http://www.gnuplot.info/documentation.html).