
# Requirements
Please download the latest version of [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) and [CMake](https://cmake.org/download/).

# Installation

## Native server (more performant):
```
> git clone --recurse-submodules https://github.com/trigonal-bacon/gardn.git
```
You will need to compile uWebSockets first. For in-depth complation options please visit the [uWebSockets installation page](https://github.com/uNetworking/uWebSockets/tree/master).
```
> cd gardn/Server/uWebSockets
> make
```
Then,
```
> cd gardn/Server
> mkdir build
> cd build
> cmake ..
> make
> ./gardn-server
```

## WebAssembly Server (doesn't require uWebSockets, but requires [Node.js](https://nodejs.org/en/download))
```
> git clone https://github.com/trigonal-bacon/gardn.git
> cd gardn/Server
> mkdir build
> cd build
> cmake .. -DWASM_SERVER=1
> make
> npm install ws fs http
> node ./gardn-server
```

Client:
```
cd gardn/Client
mkdir build
cd build
cmake ..
make
```
Then move the outputted ``wasm`` and ``js`` files into Client/public (or optionally ``Server/build`` if you're running the wasm server; make sure to move the ``html`` file as well).

The server is served by default at ``localhost:9001``. You may change the port by modifying ``Shared/Config.cc``

# Hosting 
The client may be hosted with any http server (eg. ``nginx``, ``http-server``). The wasm server automatically hosts content at ``localhost:9001`` as well.

If hosting somewhere other than ``localhost``, the ``HOSTNAME`` flag can be passed into CMake (ex. ``cmake .. -DHOSTNAME="myhost"``) to automatically configure protocol.

# License
[LICENSE](./LICENSE)