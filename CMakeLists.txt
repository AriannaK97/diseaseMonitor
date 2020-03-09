cmake_minimum_required(VERSION 3.15)
project(diseaseMonitor C)

set(CMAKE_C_STANDARD 99)

add_executable(diseaseMonitor main.c data_io.c data_io.h)