cmake_minimum_required(VERSION 3.1)

project(dotsandboxes)
set(CMAKE_CXX_STANDARD 17)

add_executable(main
  ./src/main.cpp
  ./src/dotsandboxes.cpp
  )
