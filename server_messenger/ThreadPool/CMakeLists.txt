cmake_minimum_required(VERSION 3.10)
project(ThreadPool)

add_library(${PROJECT_NAME} src/thread_pool.cpp)
add_library(sub::ThreadPool ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/../Preprocessor
)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
