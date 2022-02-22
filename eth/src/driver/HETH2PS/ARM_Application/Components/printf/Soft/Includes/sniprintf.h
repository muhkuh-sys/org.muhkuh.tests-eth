#ifndef SNIPRINTF_H_
#define SNIPRINTF_H_

#include <stdarg.h>
#include <stddef.h>

int vsniprintf(char* pBuf, size_t sizBuf, const char *pcFmt, va_list ptArgument);
int sniprintf(char* pBuf, size_t sizBuf, const char *pcFmt, ...);

#endif  /* SNIPRINTF_H_ */
