cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(server VERSION 0.19.3)

set(BOOST_ASIO_INCLUDE "PATH/TO/asio/include")
set(OPEN_SSP_INCLUDE "PATH/TO/openssl64a/include")
set(OPEN_SSP_LIB "PATH/TO/openssl64a/lib")

set(project_version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
configure_file(
    ${PROJECT_SOURCE_DIR}/main/version.hpp.in
    ${PROJECT_SOURCE_DIR}/main/version.hpp
)

add_subdirectory(PhoneNumber)
add_subdirectory(ThreadPool)
add_subdirectory(Cryptography)
add_subdirectory(Accounts)
add_subdirectory(Server)
add_subdirectory(Main)
