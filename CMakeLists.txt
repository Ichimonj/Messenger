cmake_minimum_required(VERSION 3.5)
project(server VERSION 0.3.0)


set(project_version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
configure_file(
    ${PROJECT_SOURCE_DIR}/main/version.hpp.in
    ${PROJECT_SOURCE_DIR}/main/version.hpp
)

add_subdirectory(PhoneNumber)
add_subdirectory(Accounts)
add_subdirectory(Server)

add_subdirectory(Main)

