# tictactoe
Online version of popular game.
## How
It runs with simple clients-server architecture. To play you should run server binary and two clients binaries.
## Dependencies
1. [openssl](https://github.com/openssl/openssl)
2. [toml11](https://github.com/ToruNiina/toml11)

## Install 
Add a configuration files:
```bash
mkdir configs
touch configs/config_client.toml # for client configuration

#or/and

touch configs/config_server.toml # for server configuration
```
Fill the config files:
```toml
#config_client.toml
port = 1234
server_ip = "127.0.0.1"
log_file = "logs/log_client.txt"

login = "Alice"
password = "password1"
```
```toml
#config_server.toml
port = 1234
log_file = "logs/log_server.txt"

[allowed_users]
"Alice" = "c8eb6be7da27a445473bc50d1bbf60d91e06b1332156ffdd252d87270bf351bf" #sha256 of "password1"
```
 ## Building
 ```bash
mkdir build
cd build
cmake ..
cd ..
cmake --build build
```

## How to play
The client will show you the field and you should put your symbol in blank space. To put your symbol you should send two numbers from 1 to 3:
```bash
2 2
1 3
# and so on
```
Firs number is row, second is a column where you want to make move.
## Running
```bash
./build/server
./build/client
```
