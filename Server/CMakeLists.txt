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
    Process/Drop.cc
    Process/Flower.cc
    Process/Health.cc
    Process/Motion.cc
    Process/Petal.cc
    Process/Segment.cc
    Process/Web.cc
    Client.cc
    Main.cc
    Server.cc
    Simulation.cc
    SpatialHash.cc
    Spawn.cc
    ../Shared/Arena.cc
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
target_include_directories(gardn-server PRIVATE ${CMAKE_SOURCE_DIR}/uWebSockets)
target_include_directories(gardn-server PRIVATE ${CMAKE_SOURCE_DIR}/uWebSockets/src)
target_include_directories(gardn-server PRIVATE ${CMAKE_SOURCE_DIR}/uWebSockets/uSockets/src)
target_link_directories(gardn-server PRIVATE ${CMAKE_SOURCE_DIR}/uWebSockets/uSockets)
target_link_libraries(gardn-server ws2_32 pthread z m uv uSockets ssl crypto)