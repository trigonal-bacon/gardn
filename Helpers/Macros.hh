#pragma once

#include <cassert>

#ifdef CLIENTSIDE
#define CLIENT_ONLY(...) __VA_ARGS__
#else
#define CLIENT_ONLY(...)
#endif

#ifdef SERVERSIDE
#define SERVER_ONLY(...) __VA_ARGS__
#else
#define SERVER_ONLY(...)
#endif

#ifdef DEBUG
#define DEBUG_ONLY(...) __VA_ARGS__
#else
#define DEBUG_ONLY(...)
#endif