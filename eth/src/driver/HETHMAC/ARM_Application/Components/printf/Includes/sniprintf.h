
#ifndef __SNIPRINTF_H__
#define __SNIPRINTF_H__


#include <stdarg.h>
#include <stddef.h>

int vsniprintf(char* pBuf, size_t sizBuf, const char *pcFmt, va_list ptArgument);
int sniprintf(char* pBuf, size_t sizBuf, const char *pcFmt, ...);


#endif  /* __SNIPRINTF_H__ */
