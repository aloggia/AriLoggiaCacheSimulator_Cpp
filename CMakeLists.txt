cmake_minimum_required(VERSION 3.21)
project(AriLoggiaCacheSimulator_Cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(AriLoggiaCacheSimulator_Cpp main.cpp Memory.cpp Block.cpp Set.cpp Cache.cpp GlobalFunctions.cpp)
add_executable(TagQueueTesting TagTesting.cpp)