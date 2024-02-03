# TinyAd -- Ad distribution and running system

## Init submodules
```
git submodule update --init
```

## Dependencies
```
sudo apt install libsqlite3-dev
```

## Compilation

```
cmake -DCMAKE_BUILD_TYPE=Release .
make
```

`./bin/server` -- run server

`./bin/client` -- run client(s)

`./bin/test` -- run tests

## Coverage
**Requires:**

* *gcovr*

```
sudo apt install gcovr
```

```
cmake -DCMAKE_BUILD_TYPE=Debug .
make
make coverage
```

Then open `coverage/index.html`

