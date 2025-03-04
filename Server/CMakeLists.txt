cmake_minimum_required(VERSION 3.16)
project(gardn-server)

include_directories(..)

set(SOURCES
    Process/Motion.cc
    Process/PlayerEntity.cc
    Client.cc
    Main.cc
    Server.cc
    Simulation.cc
    SpatialHash.cc
    Spawn.cc
    ../Shared/Binary.cc
    ../Shared/Entity.cc
    ../Shared/EntityDef.cc
    ../Shared/Helpers.cc
    ../Shared/Simulation.cc
    ../Shared/StaticData.cc
    ../Shared/Vector.cc
)

if(DEBUG)
    set(CMAKE_CXX_FLAGS "-std=c++20 -gdwarf-4 -lpthread -DDEBUG=1")
else()
    set(CMAKE_CXX_FLAGS "-std=c++20 -O3")
endif()
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSERVERSIDE=1")

add_executable(gardn-server ${SOURCES})
target_include_directories(gardn-server PRIVATE C:/uWebSockets/src)
target_include_directories(gardn-server PRIVATE C:/uWebSockets/uSockets/src)
target_link_directories(gardn-server PRIVATE C:/uWebSockets/uSockets)
target_link_libraries(gardn-server ws2_32 pthread z m uv uSockets ssl crypto)