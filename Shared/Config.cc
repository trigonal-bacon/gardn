#include <Shared/Config.hh>

#define STR1(arg) #arg
#define ARG_TO_STR(arg) STR1(arg)

extern const uint64_t VERSION_HASH = 4728567265382323ll;
extern const uint32_t SERVER_PORT = 9001;
extern const uint32_t MAX_NAME_LENGTH = 16;
extern const uint32_t MAX_CHAT_LENGTH = 64;
extern std::string const SERVER_HOST = "34.130.139.215";
extern std::string const WS_URL = "ws://"+SERVER_HOST+":"+std::to_string(SERVER_PORT);