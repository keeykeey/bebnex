# Bebex
Bebnex is web server.

## Build

```
cd path/to/your/bebnex
git clone git@github.com:cpputest/cpputest.git third_party/cpputest
```

```
cd path/to/your/bebnex/
mkdir build
cd build
```

build bebnex with BNX_LOG_LEVEL=3
```
cmake ../    (log level is 3 (default))
```

build bebnex with BNX_LOG_LEVEL=n(1~4)
```
cmake -DBNX_LOG_LEVEL=1 ../  (log level is 1)
cmake -DBNX_LOG_LEVEL=2 ../  (log level is 2)
cmake -DBNX_LOG_LEVEL=3 ../  (log level is 3)
cmake -DBNX_LOG_LEVEL=4 ../  (log level is 4)
```

build bebnex with debug mode
```
cmake -DBNX_DEBUG=1 ../
```

```
make
```
