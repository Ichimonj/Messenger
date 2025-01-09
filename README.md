# Описание проекта

### Серверная часть мессенджера
```server_messenger/``` 
### Клиентская часть мессенджера
```client_messenger/``` 

# Установка
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
```[ip] - ip на котором будет работать сервер```
```[port] - port на котором будет работать сервер``` 

Указывать параметры через пробел без скобок

### Вариации запуска
- Все параметры ```[program_name.exe] [number of threads with I\O] [ip] [port]```
- Количество потоков ```[program_name.exe] [number of threads with I\O]```
- Без параметров ```[program_name.exe]```

### Параметры по умолчанию
```[number of threads with I\O]``` - 12
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
