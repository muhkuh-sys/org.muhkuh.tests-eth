#ifndef UPRINTF_H_
#define UPRINTF_H_


#include <stdarg.h>
#include <stddef.h>

typedef int(*uprint_putc_fn)(unsigned int uiPort, int iChar);
typedef int(*uprint_flush_fn)(unsigned int uiPort);

typedef enum
{
  CONSOLE_LINEFEED_LF,    /* for Unix, Linux, Amiga, BeOS and MacOS X */
  CONSOLE_LINEFEED_CR,    /* for Apple II, old MacOS, OS-9 and C64 */
  CONSOLE_LINEFEED_CRLF   /* for Dos, Windows, CP/M, OS/2, and Symbian */
} CONSOLE_LINEFEED_T;

void uprint_init(uprint_putc_fn put, uprint_flush_fn flush, unsigned int uiPort);
void uprint_lfmode(CONSOLE_LINEFEED_T tMode);

int  uprint(const char *pszStr);
void uprintf(const char *pcFmt, ...);
void uprintln(const char *pcFmt, ...);
void vuprintf(const char *pcFmt, va_list argp);
void vuprintln(const char *pcFmt, va_list argp);
void hexdump(const unsigned char *pcData, size_t sizData);

#endif  /* UPRINTF_H_ */
