cmake_minimum_required(VERSION 3.10)
project(server VERSION 0.2.0)

set(project_version ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
configure_file(
    ${PROJECT_SOURCE_DIR}/main/version.hpp.in
    ${PROJECT_SOURCE_DIR}/main/version.hpp
)

add_subdirectory(Account)
add_subdirectory(ChatManager)
add_subdirectory(Console)
add_subdirectory(Error)
add_subdirectory(main)