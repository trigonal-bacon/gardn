cmake_minimum_required(VERSION 3.16)
project(gardn-server)

# Project-wide include dir
include_directories(..)

set(SOURCES
    EntityFunctions/Damage.cc
    EntityFunctions/Death.cc
    EntityFunctions/Detection.cc
    EntityFunctions/Misc.cc
    Process/Ai.cc
    Process/Camera.cc
    Process/Collision.cc
    Process/Culling.cc
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
    Stats.cc
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

# Add executable
add_executable(gardn-server ${SOURCES})

# Set C++20 standard properly (modern way)
target_compile_features(gardn-server PUBLIC cxx_std_20)

# Compile definitions
target_compile_definitions(gardn-server PRIVATE SERVERSIDE=1)

# Compile options per build type
if(DEBUG)
    target_compile_options(gardn-server PRIVATE -gdwarf-4 -lpthread -DDEBUG=1)
else()
    target_compile_options(gardn-server PRIVATE -O3 -ffast-math)
endif()

# Include directories for target
target_include_directories(gardn-server PRIVATE
    ${CMAKE_SOURCE_DIR}/uWebSockets/src
    ${CMAKE_SOURCE_DIR}/uWebSockets/uSockets/src
    ..
)

# Link directories and libraries
target_link_directories(gardn-server PRIVATE ${CMAKE_SOURCE_DIR}/uWebSockets/uSockets)

target_link_libraries(gardn-server
    uv
    z
    -l:uSockets.a
)

# Try to link libcurl if available for webhook support
find_package(CURL QUIET)
if (CURL_FOUND)
    target_include_directories(gardn-server PRIVATE ${CURL_INCLUDE_DIRS})
    target_link_libraries(gardn-server ${CURL_LIBRARIES})
    target_compile_definitions(gardn-server PRIVATE HAS_LIBCURL=1)
endif()

if(CMAKE_HOST_WIN32)
    target_link_libraries(gardn-server ws2_32)
endif()

# WASM build support
if(WASM_SERVER)
    set(CMAKE_CXX_COMPILER "em++")  # set compiler early if needed
    target_compile_definitions(gardn-server PRIVATE WASM_SERVER=1)
    add_link_options(-sEXIT_RUNTIME=0 -sEXPORTED_FUNCTIONS=_main,_on_connect,_on_disconnect,_tick,_on_message)
    if(NOT DEBUG)
        target_compile_options(gardn-server PRIVATE --closure=1)
    endif()
endif()
