cmake_minimum_required(VERSION 3.10)
project(Error)

set(SOURCES
    src/error.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::Error ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC sub::Console
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Console/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Settings
)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
