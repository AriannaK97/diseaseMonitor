cmake_minimum_required(VERSION 3.15)
project(diseaseMonitor C)

set(CMAKE_C_STANDARD 99)

add_executable(diseaseMonitor src/main.c src/data_io.c header/data_io.h src/list_lib.c header/list_lib.h src/command_lib.c header/command_lib.h src/hashTable.c
        header/hashTable.h src/redBlackTree.c header/redBlackTree.h header/structs.h src/binaryMaxHeap.c header/binaryMaxHeap.h)