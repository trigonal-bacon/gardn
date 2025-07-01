# the gardn project
Open source self-hostable, multiplayer version of pvp [florr.io](https://static.florr.io/old/) written in C++.

See [INSTALLATION.md](./INSTALLATION.md) for how to set up and run the game.

# About
Although this game is heavily inspired from florr.io, all lines of code were personally written by me. As direct source code of the original game is not accessible, all game logic is also custom-made to fit the original as closely as possible, while allowing for some creativity.

## ECS
This game uses a self-made [entity component system](https://en.wikipedia.org/wiki/Entity_component_system) engine for the simulation and entities. Entity property macros are defined in [Shared/EntityDef.hh](./Shared/EntityDef.hh), and the entity's structure is built in [Shared/Entity.hh](./Shared/EntityDef.hh). The simulation also defines functions to run systems/processes, a crucial component of ECS.

## Server
The server can be run either natively using uWebSockets or through Node.js and WebAssembly. This allows for an alternate installation process in case uWebSockets configuration fails.

Collision detection is managed using a uniform grid, which capitalizes on the relatively uniform entity sizes to provide fast broadphase collision detection and querying.

## Client

The client is written in C++, which compiles to WASM/JS using emscripten. Apart from a text input box, all rendering is done using a bootstrap of the [Canvas2D API](https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D) (see [Client/Render/Renderer.cc](./Client/Render/Renderer.cc)).

For performance, none of the UI is written in HTML or JS. I have written a separate UI layout/rendering/event firing engine in [Client/Ui](./Client/Ui/) that handles all UI logic and simulates common HTML elements. It supports all animations seen in the original game, and some extras.

Assets are directly copied from the game using several userscripts (see [Scripts](./Scripts/)). Several mobs have animations, which were reversed directly from the WASM.
