set(CMAKE_SYSTEM_NAME Generic)

cmake_minimum_required(VERSION 3.16)

project(gardn-client)
include_directories(..)

set(SRCS
    Assets/Mob.cc
    Assets/Petal.cc
    Assets/Web.cc
    Render/RenderDrop.cc
    Render/RenderFlower.cc
    Render/RenderHealth.cc
    Render/RenderMob.cc
    Render/RenderPetal.cc
    Render/RenderWeb.cc
    Render/Renderer.cc
    Game.cc
    Input.cc
    Main.cc
    Simulation.cc
    Socket.cc
    ../Shared/Binary.cc
    ../Shared/Entity.cc
    ../Shared/EntityDef.cc
    ../Shared/Helpers.cc
    ../Shared/Simulation.cc
    ../Shared/StaticData.cc
    ../Shared/Vector.cc
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCLIENTSIDE=1 -std=c++20")
set(CMAKE_CXX_COMPILER "em++")
if(DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG=1 -gdwarf-4 -sNO_DISABLE_EXCEPTION_CATCHING")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math --closure=1")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_EXIT_RUNTIME=1 -s EXPORTED_FUNCTIONS=_main,_malloc,_free,_key_event,_mouse_event,_loop,_on_message")

add_executable(gardn-client ${SRCS})

target_link_libraries(gardn-client m)