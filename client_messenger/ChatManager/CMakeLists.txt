cmake_minimum_required(VERSION 3.10)
project(ChatManager)

set(SOURCES
    src/ChatManager.cpp
    src/Command.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::ChatManager ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC sub::Console
    PUBLIC sub::Error
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Console/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Error/include
    PUBLIC D:/vs/newlibs/asio-1.18.0/include
)