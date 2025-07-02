set(CMAKE_SYSTEM_NAME Generic)

cmake_minimum_required(VERSION 3.16)
project(gardn-server)

include_directories(..)

set(SOURCES
    EntityFunctions/Damage.cc
    EntityFunctions/Death.cc
    EntityFunctions/Detection.cc
    EntityFunctions/Misc.cc
    Process/Ai.cc
    Process/Camera.cc
    Process/Collision.cc
    Process/Flower.cc
    Process/Health.cc
    Process/Motion.cc
    Process/Petal.cc
    Process/Segment.cc
    Client.cc
    Main.cc
    Native.cc
    PetalTracker.cc
    Server.cc
    Simulation.cc
    SpatialHashUniform.cc
    Spawn.cc
    Wasm.cc
    ../Shared/Arena.cc
    ../Shared/Binary.cc
    ../Shared/Config.cc
    ../Shared/Entity.cc
    ../Shared/EntityDef.cc
    ../Shared/Helpers.cc
    ../Shared/Map.cc
    ../Shared/Simulation.cc
    ../Shared/StaticData.cc
    ../Shared/Vector.cc
)

if(DEBUG)
    set(CMAKE_CXX_FLAGS "-gdwarf-4 -lpthread -DDEBUG=1")
else()
    set(CMAKE_CXX_FLAGS "-O3 -ffast-math")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -DSERVERSIDE=1")

add_executable(gardn-server ${SOURCES})
if(WASM_SERVER)
    set(CMAKE_CXX_COMPILER "em++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWASM_SERVER=1 -s NO_EXIT_RUNTIME=1 -s EXPORTED_FUNCTIONS=_main,_on_connect,_on_disconnect,_tick,_on_message")
    target_link_libraries(gardn-server m)
else()
    set(CMAKE_CXX_COMPILER "g++")
    target_include_directories(gardn-server PRIVATE ${CMAKE_SOURCE_DIR}/uWebSockets/src)
    target_include_directories(gardn-server PRIVATE ${CMAKE_SOURCE_DIR}/uWebSockets/uSockets/src)
    target_link_directories(gardn-server PRIVATE ${CMAKE_SOURCE_DIR}/uWebSockets/uSockets)
    target_link_libraries(gardn-server uv z)
    target_link_libraries(gardn-server -l:uSockets.a)
    if(CMAKE_HOST_WIN32)
        target_link_libraries(gardn-server ws2_32)
    endif()
endif()