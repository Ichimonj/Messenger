project(Crypto)

set(SOURCES
    src/hash.cpp
    src/rand.cpp
)

add_library(${PROJECT_NAME} ${SOURCES}) 
add_library(sub::Crypto ALIAS ${PROJECT_NAME})  

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${OPEN_SSP_INCLUDE}
)
target_link_directories(${PROJECT_NAME} PUBLIC ${OPEN_SSP_LIB})
target_link_libraries(${PROJECT_NAME} libcrypto.lib libssl.lib)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../lib
)
