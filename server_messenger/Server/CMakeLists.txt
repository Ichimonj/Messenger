cmake_minimum_required(VERSION 3.10)
project(Server)

set(SOURCES
    src/account_base.cpp
    src/account_factory.cpp
    src/server.cpp
    src/chat_manager.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::Server ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC sub::Accounts
    PUBLIC sub::ThreadPool
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Accounts/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../ThreadPool/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Files
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
