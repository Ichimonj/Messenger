cmake_minimum_required(VERSION 3.10)
project(ChatManager)

set(SOURCES
    src/chat_manager.cpp
    src/command.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
add_library(sub::ChatManager ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC sub::Console
    PUBLIC sub::Error
    PUBLIC sub::FileWork
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Console/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../Error/include
    PUBLIC ${PROJECT_SOURCE_DIR}/../FileWork/include
    PUBLIC ${BOOST_ASIO_INCLUDE}
)
install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
