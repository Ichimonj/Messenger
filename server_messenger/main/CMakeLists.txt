cmake_minimum_required(VERSION 3.10)
project(Main)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(Main PRIVATE crypt32)

target_link_libraries(${PROJECT_NAME}
    PRIVATE sub::PhoneNumber
    PRIVATE sub::Accounts
    PRIVATE sub::Server
)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${PROJECT_SOURCE_DIR}/../bin
)
file(WRITE ${PROJECT_SOURCE_DIR}/../bin/start_help.txt 
"Запуск сервера
Параметры запуска
[program_name.exe] [number of threads with I\\O] [ip] [port]
[number of threads with I\\O] - количество потоков для I\\O context
[ip] - ip на котором будет работать сервер
[port] - port на котором будет работать сервер\n
Указывать параметры через пробел без скобок\n
Вариации запуска
- Все параметры [program_name.exe] [number of threads with I\\O] [ip] [port]
- Количество потоков [program_name.exe] [number of threads with I\\O]
- Без параметров [program_name.exe]\n
Параметры по умолчанию
[number of threads with I\\O] - 12
[ip] - 127.0.0.1
[port] - 8080"
)