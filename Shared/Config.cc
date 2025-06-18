#include <Shared/Config.hh>

#define STR1(arg) #arg
#define ARG_TO_STR(arg) STR1(arg)

extern std::string const HOST_NAME = 
#ifdef HOSTNAME
ARG_TO_STR(HOSTNAME);
#else
"localhost";
#endif
#undef STR1
#undef ARG_TO_STR
extern const uint64_t VERSION_HASH = 198456321345ll;
extern const uint32_t SERVER_PORT = 9001;