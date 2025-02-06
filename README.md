# Описание проекта

### Серверная часть мессенджера
```server_messenger/``` 
### Клиентская часть мессенджера
```client_messenger/``` 

# Установка
## Установка библиотек
В файлах `CMakeLists.txt`
нужно аменить пути в
```cmake
set(BOOST_ASIO_INCLUDE "PATH/TO/asio/include")
set(OPEN_SSP_INCLUDE "PATH/TO/openssl64a/include")
set(OPEN_SSP_LIB "PATH/TO/openssl64a/lib")
```
на соотвествующие пути.
## Установка серверной части
```bash
cd server_messenger
build
```
## Конечные папки
```server_messenger/bin```
```server_messenger/lib``` 

## Запуск сервера
### Параметры запуска
```[program_name.exe] [number of threads with I\O] [ip] [port]```
```[number of threads with I\O] - количество потоков для I\O context```
```[number of threads with thread pool] - количество потоков для пула потоков```
```[ip] - ip на котором будет работать сервер```
```[port] - port на котором будет работать сервер``` 

Указывать параметры через пробел без скобок

### Вариации запуска
(В текущей версии Thread pool не интегрирован)
- Все параметры ```[program_name.exe] [number of threads with I\O] [ip] [port]```
- Количество потоков I\O ```[program_name.exe] [number of threads with I\O]```
- Количество потоков I\O и пула потоков: 
```[program_name.exe] [number of threads with I\O] [number of threads with thread pool]```
- Без параметров ```[program_name.exe]```

### Параметры по умолчанию
```[number of threads with I\O]``` - 12
```[number of threads with thread pool]``` - 12
```[ip]``` - 127.0.0.1
```[port]``` - 8080
## Установка клиентской части
```bash
cd client_messenger
build
```
## Конечные папки
```client_messenger/bin```
```client_messenger/lib```
