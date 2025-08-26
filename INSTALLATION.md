
# Requirements
Please download the latest version of [Emscripten](https://emscripten.org/docs/getting_started/downloads.html) and [CMake](https://cmake.org/download/). You will also need the latest version of gcc/g++ (>=c++20).

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
> node ./gardn-server.js
```

## Client:
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

If hosting somewhere other than ``localhost``, the ``SERVER_URL`` flag can be passed into CMake (ex. ``cmake .. -DSERVER_URL="ws://myhost:9001"``) to automatically configure protocol.

# Compilation Flags

``DEBUG`` | ``Server & Client`` | ``Default: 0`` : compiles with assertions and failsafes. <br>
``WASM_SERVER`` | ``Server only`` | ``Default : 0`` : compiles to WASM/JS instead of a native binary. <br>
``TDM`` | ``Server only`` | ``Default: 0`` : enables TDM instead of FFA.<br>
``GENERAL_SPATIAL_HASH`` | ``Server only`` | ``Default: 0`` : uses the canonical hash grid implementation instead of a uniform grid; enable this to support large entities. <br>
``USE_CODEPOINT_LEN`` | ``Server & Client`` | ``Default: 0`` : uses the number of codepoints (characters) instead of byte length for string validation and truncation - useful for non-english characters. Should be the same on both server and client.

# License
[LICENSE](./LICENSE)