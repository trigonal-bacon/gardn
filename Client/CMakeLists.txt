set(CMAKE_SYSTEM_NAME Generic)

cmake_minimum_required(VERSION 3.16)

project(gardn-client)
include_directories(..)

set(SRCS
    Assets/Flower.cc
    Assets/Mob.cc
    Assets/Petal.cc
    Assets/Web.cc
    Render/RenderDrop.cc
    Render/RenderFlower.cc
    Render/RenderHealth.cc
    Render/RenderMob.cc
    Render/RenderName.cc
    Render/RenderPetal.cc
    Render/RenderWeb.cc
    Render/Renderer.cc
    Debug.cc
    DOM.cc
    Game.cc
    Input.cc
    Main.cc
    Network.cc
    Particle.cc
    Rendering.cc
    Setup.cc
    Simulation.cc
    Socket.cc
    Storage.cc
    Ui/InGame/InputFreeze.cc
    Ui/InGame/Leaderboard.cc
    Ui/InGame/LevelBar.cc
    Ui/InGame/LoadoutPetal.cc
    Ui/InGame/LoadoutSlot.cc
    Ui/InGame/Map.cc
    Ui/InGame/OverlevelTimer.cc
    Ui/InGame/Tooltip.cc
    Ui/TitleScreen/Changelog.cc
    Ui/TitleScreen/DeathScreen.cc
    Ui/TitleScreen/MainScreen.cc
    Ui/TitleScreen/MobGallery.cc
    Ui/TitleScreen/PetalGallery.cc
    Ui/TitleScreen/Settings.cc
    Ui/TitleScreen/StatScreen.cc
    Ui/Button.cc
    Ui/Choose.cc
    Ui/Container.cc
    Ui/DynamicText.cc
    Ui/Element.cc
    Ui/Extern.cc
    Ui/ScrollContainer.cc
    Ui/StaticText.cc
    Ui/TextInput.cc
    Ui/Window.cc
    ../Shared/Arena.cc
    ../Shared/Binary.cc
    ../Shared/Entity.cc
    ../Shared/EntityDef.cc
    ../Shared/Helpers.cc
    ../Shared/Map.cc
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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_EXIT_RUNTIME=1 -s EXPORTED_FUNCTIONS=_main,_malloc,_free,_key_event,_mouse_event,_wheel_event,_loop,_on_message")

add_executable(gardn-client ${SRCS})

target_link_libraries(gardn-client m)