cmake_minimum_required(VERSION 3.28)
project(Venus C)

set(CMAKE_C_STANDARD 11)

#target_include_directories(libpynq PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libpynq-5EID0-2023-v0.3.0)

add_executable(Venus
        AlgorithmTestCases.c
        all_sensors_test.c
        BFStest.c
        distance.c
        distance.h
        embeddedInitialImp.c
        embeddedInitialImp.h
        examplecode.c
        IR_sensor.c
        IR_sensor.h
        esp_2)
