cmake_minimum_required(VERSION 3.5)
project(LinearRegression VERSION 0.1.0 LANGUAGES C CXX)

set(SOURCE_FILES
    main.cpp
    regression-analysis/regression.cpp
)

set(HEADER_FILES
    regression-analysis/include/regression.h
)

include(CTest)
enable_testing()

add_subdirectory(matplotplusplus)

add_executable(main ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(main PUBLIC matplot)

# Add this line
target_include_directories(main PUBLIC regression-analysis/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)