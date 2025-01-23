cmake_minimum_required(VERSION 3.10)
project(Account)

set(SOURCES
    src/account.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::Account ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC sub::ChatManager
    PUBLIC sub::Console
    PUBLIC sub::Error
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../ChatManager/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Console/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Error/include
    PUBLIC D:/vs/newlibs/asio-1.18.0/include
)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
