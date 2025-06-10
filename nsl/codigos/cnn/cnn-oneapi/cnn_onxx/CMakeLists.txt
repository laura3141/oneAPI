cmake_minimum_required(VERSION 3.28)
project(IntrusionDetectionONNX)

set(CMAKE_CXX_STANDARD 17)

# Altere este caminho para onde você compilou o ONNX Runtime
set(ONNXRUNTIME_DIR "/home/arthurdias/Documents/onnx/onnxruntime/build/Linux/Release")

include_directories(${ONNXRUNTIME_DIR}/include)
link_directories(${ONNXRUNTIME_DIR})

add_executable(intrusion main.cpp)

target_link_libraries(intrusion onnxruntime)
