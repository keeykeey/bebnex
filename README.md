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
#### if you want debug with gdb, run ./configure with --debug=gdb option.
    --debug=gdb
### 3 ./auto/make
####

### 4 make install