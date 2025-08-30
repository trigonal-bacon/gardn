#include <Shared/Config.hh>

extern const uint64_t VERSION_HASH = 
#ifdef BUILD_TIME
BUILD_TIME;
#else
0ull;
#endif

extern const uint32_t SERVER_PORT = 9001;
extern const uint32_t MAX_NAME_LENGTH = 16;
extern const uint32_t MAX_CHAT_LENGTH = 64;
extern std::string const WS_URL = 
#ifdef SERVER_URL
SERVER_URL;
#else
"ws://localhost:"+std::to_string(SERVER_PORT);
#endif