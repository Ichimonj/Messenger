cmake_minimum_required(VERSION 3.10)
project(PhoneNumber)

add_library(${PROJECT_NAME} src/PhoneNumber.cpp)
add_library(sub::PhoneNumber ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Preprocessor
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
