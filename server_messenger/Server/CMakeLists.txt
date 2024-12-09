cmake_minimum_required(VERSION 3.10)
project(Server)

set(SOURCES
    src/AccountBase.cpp
    src/AccountFactory.cpp
    src/Server.cpp
    src/ChatManager.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::Server ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC sub::Accounts
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Accounts/include
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
