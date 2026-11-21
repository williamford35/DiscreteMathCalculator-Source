#include <string.h>

#ifdef _WIN64
char *strsep (char **stringp, const char *delim);
#endif

size_t strlcpy (char *dst, const char *src, size_t siz);
