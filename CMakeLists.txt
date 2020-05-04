cmake_minimum_required(VERSION 2.8)

project(cuda)

set(CMAKE_CXX_FLAGS_RELEASE "-O3" )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# set (CMAKE_BUILD_TYPE Release)

# Sources
include_directories("src")
file(GLOB SOURCES "src/*.h" "src/*.hpp" "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} pthread -lgomp)

# OpenMP
find_package(OpenMP)
if (OpenMP_CXX_FOUND)
	target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
endif()

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# CUDA
include(FindCUDA)
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})
set(CUDA_NVCC_FLAGS "-arch=sm_30")
cuda_add_library(blur STATIC src/gaussian.cu)
target_link_libraries(${PROJECT_NAME} blur ${CUDA_LIBRARIES})
