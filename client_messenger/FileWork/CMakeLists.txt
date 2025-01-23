cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(FileWork)

set(SOURCES
    src/file_work.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::FileWork ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
