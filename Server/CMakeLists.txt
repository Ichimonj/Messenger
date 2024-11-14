project(Server)

set(SOURCES
    src/AccountBase.cpp
    src/AccountFactory.cpp
    src/Server.cpp
)
add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::Server ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Accounts/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Preprocessor
    PUBLIC ${PROJECT_SOURCE_DIR}/../PhoneNumber/include
    PUBLIC D:/vs/newlibs/asio-1.18.0/include
)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
