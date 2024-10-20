# Bebnex
## How to build
### 1 set the config-file at any path you like
#### config-file would be like below
    PREFIX=/var/www/index.html
    PORT=8080`
### 2 ./auto/configure --conf-file="your config-file-path" ...
#### options would be like below
    --conf-file=/etc/bebnex/bebnex.conf
    --build-path=/usr/local/bin/
    --access-log-file=/var/log/bebnex/bebnex.access.log
#### if you want to debug with gdb, run ./configure with --debug=gdb option.
    --debug=gdb
#### if you want to use ipv6, run ./configure with -ipv6 option.
    -ipv6
### 3 ./auto/make
####

### 4 make install

## tested platform
    1 ubuntu22.0.4
    2 ubuntu24.0.4
    3 windows server2022

## bebnex is supported only to a limitted extend on windows
### these features are not available on windows
    using ipv6