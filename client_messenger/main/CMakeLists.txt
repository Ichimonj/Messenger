cmake_minimum_required(VERSION 3.10)
project(Main)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(Main PRIVATE crypt32)

target_link_libraries(${PROJECT_NAME}
    PRIVATE sub::Account
    PRIVATE sub::ChatManager
    PRIVATE sub::Console
    PRIVATE sub::Error
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../bin
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Account/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Console/include
    PUBLIC D:/vs/newlibs/asio-1.18.0/include
)