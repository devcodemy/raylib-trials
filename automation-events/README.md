## Clean setup ^dev
```shell
cmake -S . -B build

cd build
make

```

## Apple M1
```shell
# need to check
mkdir build && cd build 
cmake -DCMAKE_APPLE_SILICON_PROCESSOR=arm64 .. 
make
```