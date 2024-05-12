cmake_minimum_required(VERSION 3.5)
project(LinearRegression VERSION 0.1.0 LANGUAGES C CXX)

set(SOURCE_FILES
    main.cpp
    regression-analysis/src/regression.cpp
    regression-analysis/src/GridSearch.cpp
    regression-analysis/src/RandomSearch.cpp
    regression-analysis/src/save.cpp
    Data/src/Analysis.cpp
    Data/src/Data.cpp
)

set(HEADER_FILES
    regression-analysis/include/interface.h
    regression-analysis/include/regression.h
    regression-analysis/include/optimization.h
    regression-analysis/include/HyperParameterOptimization.h
    Data/include/Data.h
    Data/include/Analysis.h
)

include(CTest)
enable_testing()


add_executable(main ${SOURCE_FILES} ${HEADER_FILES})
 
#target_link_libraries(main PUBLIC matplot)

#add_subdirectory(matplotplusplus)

target_include_directories(main PUBLIC regression-analysis/include Data/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)