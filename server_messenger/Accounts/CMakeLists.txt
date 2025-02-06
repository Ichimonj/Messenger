cmake_minimum_required(VERSION 3.10)
project(Accounts)

set(SOURCES
    src/abstract_account.cpp
    src/temp_account.cpp
    src/user_account.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::Accounts ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC sub::Crypto
    PUBLIC sub::PhoneNumber
    PUBLIC sub::ThreadPool
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../PhoneNumber/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Server/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Preprocessor
    PUBLIC ${PROJECT_SOURCE_DIR}/../Cryptography/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../ThreadPool/include
    PUBLIC ${BOOST_ASIO_INCLUDE}
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
