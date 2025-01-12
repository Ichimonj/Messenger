cmake_minimum_required(VERSION 3.10)
project(ThreadPool)

add_library(${PROJECT_NAME} src/ThreadPool.cpp)
add_library(sub::ThreadPool ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
