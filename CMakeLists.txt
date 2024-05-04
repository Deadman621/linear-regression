cmake_minimum_required(VERSION 3.5)
project(LinearRegression VERSION 0.1.0 LANGUAGES C CXX)

set(SOURCE_FILES
    main.cpp
    regression-analysis/src/regression.cpp
    regression-analysis/src/GridSearch.cpp
    regression-analysis/src/RandomSearch.cpp
    Data/src/Data.cpp
    Data/src/Analysis.cpp
)

set(HEADER_FILES
    regression-analysis/include/regression.h
    regression-analysis/include/optimization.h
    regression-analysis/include/HyperParameterOptimization.h
    Data/include/Data.h
    Data/include/Analysis.h
)

include(CTest)
enable_testing()

#add_subdirectory(matplotplusplus)

add_executable(main ${SOURCE_FILES} ${HEADER_FILES})
#target_link_libraries(main PUBLIC matplot)

# Add this line
target_include_directories(main PUBLIC regression-analysis/include Data/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)