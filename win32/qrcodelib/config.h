

#define MAJOR_VERSION 3
#define MINOR_VERSION 9
#define MICRO_VERSION 0
#define VERSION "3.9.0"

#define __STATIC static

#if defined(_WIN32) || defined(_WIN64)
  #define snprintf _snprintf
  #define vsnprintf _vsnprintf
  #define strcasecmp _stricmp
  #define strncasecmp _strnicmp
  #define strdup _strdup
#endif