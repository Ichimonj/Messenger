cmake_minimum_required(VERSION 3.10)
project(Console)

set(SOURCES
    src/console.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::Console ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC sub::ChatManager
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../ChatManager/include
    PUBLIC D:/vs/newlibs/asio-1.18.0/include
)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
