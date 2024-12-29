cmake_minimum_required(VERSION 3.10)
project(Main)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(Main PRIVATE crypt32)

target_link_libraries(${PROJECT_NAME}
    PRIVATE sub::PhoneNumber
    PRIVATE sub::Accounts
    PRIVATE sub::Server
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../bin
)
