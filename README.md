# Key-Value in memory database

This project is a experimental key-value in memory database, written in C.
It's far from being a production ready database, but it's a good way to learn.
There is some features that I want to implement in the future, like:

- Configurable parameters like port, max connections, etc.
- Better error handling
- Better logging
- Stats
- Rewrite some parts to better legibility

## Install

## Linux

```sh
sudo apt install build-essential cmake python3
```

## Mac

```sh
xcode-select --install
brew install cmake
brew install python3
```

## Build

```sh
mkdir build
cd build
cmake ..
make
```

## Test

```sh
cd build\
make test
```

run the integration test make sure that the server is running and then:

```sh
python3 ./scripts/client.py
```

## Usage

```sh
./bin/mendb
```

in other terminal

```sh
nc localhost 8080
SET key value
OK

GET key
"value"

DELETE key
OK

GET key
(nil)
```

## License

MIT License © [Rafael Castro](https://github.com/rafaelc457ro)
