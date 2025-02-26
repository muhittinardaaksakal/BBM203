cmake_minimum_required(VERSION 3.29)
project(ass3)

set(CMAKE_CXX_STANDARD 20)

# Add the executable target
add_executable(ass3
        main.cpp
        RoutePlanner.cpp
        Map.cpp
        Queue.cpp
        Stack.cpp
)
