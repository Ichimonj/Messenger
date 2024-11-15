project(Crypto)

set(SOURCES
    src/Hash.cpp
)

add_library(${PROJECT_NAME} ${SOURCES}) 
add_library(sub::Crypto ALIAS ${PROJECT_NAME})  

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE D:/vs/newlibs/openssl64a/include
)

find_library(openSSL NAMES test HINTS D:/vs/newlibs/openssl64a/lib)
target_link_libraries(${PROJECT_NAME} ${openSSL})

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)