# Bebnex
## How to build
### 1 configure(creating Makefile)
    $ ./auto/configure [...options]
#### options would be like below
    --prefix=/usr/local/bebnex/
    --conf-file=bebnex.conf
    --access-log-file=bebnex.access.log
#### if you want to debug with gdb, run ./configure with --debug=gdb option.
    --debug=gdb
#### if you want to use ipv6, run ./configure with -ipv6 option.
    -ipv6
### 2 create bebnex(binary file)
    $ make
####

### 3 install bebnex
    $ make install

## tested platform
    1 ubuntu22.0.4
    2 ubuntu24.0.4
    3 windows server2022

## bebnex is supported only to a limitted extend on windows
### these features are not available on windows
    using ipv6