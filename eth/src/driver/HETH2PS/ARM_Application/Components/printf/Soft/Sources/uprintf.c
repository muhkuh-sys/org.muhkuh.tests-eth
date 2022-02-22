#include "uprintf.h"
#include <string.h>

#define SERIAL_PUT(x)  s_tStdout.put(s_tStdout.uiPort, x)
#define SERIAL_FLUSH() if(s_tStdout.flush)s_tStdout.flush(s_tStdout.uiPort)

typedef struct
{
  uprint_putc_fn put;
  uprint_flush_fn flush;
  unsigned int uiPort;
} UPRINT_STREAM_T;

int uprint_putchar_dummy(unsigned int uiPort, int i){ return -1; }
static UPRINT_STREAM_T s_tStdout = { uprint_putchar_dummy, NULL, 0 };
static CONSOLE_LINEFEED_T s_tLinefeedMode = CONSOLE_LINEFEED_CRLF;

void uprint_init(uprint_putc_fn put, uprint_flush_fn flush, unsigned int uiPort)
{
  s_tStdout.put = put;
  s_tStdout.flush = flush;
  s_tStdout.uiPort = uiPort;
}

void uprint_lfmode(CONSOLE_LINEFEED_T tMode)
{
  s_tLinefeedMode = tMode;
}

static void print_linefeed(void)
{
  /* print linefeed */
  switch(s_tLinefeedMode)
  {
  case CONSOLE_LINEFEED_LF:
    SERIAL_PUT('\n');
    break;

  case CONSOLE_LINEFEED_CR:
    SERIAL_PUT('\r');
    break;

  default:
  case CONSOLE_LINEFEED_CRLF:
    SERIAL_PUT('\r');
    SERIAL_PUT('\n');
    break;
  }
  SERIAL_FLUSH();
}

static void uprintf_hex(unsigned long ulValue, size_t sizMinimum, char cFillUpChar)
{
  int fLeadingDigitWasPrinted;
  size_t sizCnt;
  unsigned int uiDigit;


  /* the maximum size is 8 chars */
  sizCnt = 8;

  /* no leading digit was printed yet */
  fLeadingDigitWasPrinted = 0;

  /* loop over all possible chars */
  do
  {
    --sizCnt;

    /* get top digit */
    uiDigit = ulValue >> 28;
    if( uiDigit==0 && fLeadingDigitWasPrinted==0 && sizCnt>0 )
    {
      /* already reached the minimum display size? */
      if( sizCnt<sizMinimum )
      {
        /* yes -> print fillup char */
        SERIAL_PUT(cFillUpChar);
      }
    }
    else
    {
      /* print the digit */
      uiDigit |= '0';
      if( uiDigit>'9' )
      {
        uiDigit += 'a'-'9'-1;
      }
      SERIAL_PUT((unsigned char)uiDigit);

      /* now the leading digit has been printed */
      fLeadingDigitWasPrinted = 1;
    }

    /* move to next digit */
    ulValue <<= 4;
  } while( sizCnt>0 );
}


static const unsigned long aulUprintfDecTab[10] =
{
           1,
          10,
         100,
        1000,
       10000,
      100000,
     1000000,
    10000000,
   100000000,
  1000000000
};

static void uprintf_dec(unsigned long ulValue, size_t sizMinimum, char cFillUpChar)
{
  int fLeadingDigitWasPrinted;
  size_t sizCnt;
  unsigned int uiDigit;


  /* the maximum size is 10 chars */
  sizCnt = 10;

  /* no leading digit was printed yet */
  fLeadingDigitWasPrinted = 0;

  /* loop over all possible chars */
  do
  {
    --sizCnt;

    /* get top digit */
    uiDigit = 0;
    while( ulValue>=aulUprintfDecTab[sizCnt] )
    {
      ulValue -= aulUprintfDecTab[sizCnt];
      ++uiDigit;
    }

    if( uiDigit==0 && fLeadingDigitWasPrinted==0 && sizCnt>0 )
    {
      /* already reached the minimum display size? */
      if( sizCnt<sizMinimum )
      {
        /* yes -> print fillup char */
        SERIAL_PUT(cFillUpChar);
      }
    }
    else
    {
      /* print the digit */
      uiDigit |= '0';
      SERIAL_PUT((unsigned char)uiDigit);

      /* now the leading digit has been printed */
      fLeadingDigitWasPrinted = 1;
    }
  } while( sizCnt>0 );
}


static void uprintf_bin(unsigned long ulValue, size_t sizMinimum, char cFillUpChar)
{
  int fLeadingDigitWasPrinted;
  size_t sizCnt;
  unsigned int uiDigit;


  /* the maximum size is 32 chars */
  sizCnt = 32;

  /* no leading digit was printed yet */
  fLeadingDigitWasPrinted = 0;

  /* loop over all possible chars */
  do
  {
    --sizCnt;

    /* get top digit */
    uiDigit = ulValue >> 31;
    if( uiDigit==0 && fLeadingDigitWasPrinted==0 && sizCnt>0 )
    {
      /* already reached the minimum display size? */
      if( sizCnt<sizMinimum )
      {
        /* yes -> print fillup char */
        SERIAL_PUT(cFillUpChar);
      }
    }
    else
    {
      /* print the digit */
      uiDigit |= '0';
      SERIAL_PUT((unsigned char)uiDigit);

      /* now the leading digit has been printed */
      fLeadingDigitWasPrinted = 1;
    }

    /* move to next digit */
    ulValue <<= 1;
  } while( sizCnt>0 );
}


static void uprintf_str(const char *pcString, size_t sizMinimum, char cFillUpChar)
{
  size_t sizString;
  size_t sizCnt;
  const char *pcCnt;
  char cChar;

  if(pcString == NULL)
    return;

  /* get the length of the string */
  sizString = 0;
  pcCnt = pcString;
  while( *(pcCnt++)!='\0' )
  {
    ++sizString;
  }

  /* fillup string if it is smaller than the requested size */
  sizCnt = sizString;
  while( sizCnt<sizMinimum )
  {
    SERIAL_PUT(cFillUpChar);
    ++sizCnt;
  }

  /* print the string */
  pcCnt = pcString;
  do
  {
    cChar = *(pcCnt++);
    if( cChar!='\0' )
    {
      SERIAL_PUT(cChar);
    }
  } while( cChar!='\0' );
}

void vuprintf(const char *pcFmt, va_list ptArgument)
{
  char cChar;
  char cFillUpChar;
  size_t sizMinimumSize;
  int iDigitCnt;
  unsigned int uiCnt;
  unsigned int uiValue;
  const char *pcNumCnt;
  const char *pcNumEnd;
  unsigned long ulArgument;
  const char *pcArgument;
  int iArgument;

  /* Is it a NULL Pointer ? */
  if( pcFmt==NULL )
  {
    /* replace the argument with the default string */
    pcFmt = "NULL\n";
  }

  /* loop over all chars in the format string */
  do
  {
    /* get the next char */
    cChar = *(pcFmt++);

    /* is this the end of the format string? */
    if( cChar!=0 )
    {
      /* no -> process the char */

      /* is this an escape char? */
      if( cChar=='%' )
      {
        /* yes -> process the escape sequence */

        /* set default values for escape sequences */
        cFillUpChar = ' ';
        sizMinimumSize = 0;

        do
        {
          cChar = *(pcFmt++);
          if( cChar=='%' )
          {
            /* it is just a '%' */
            SERIAL_PUT(cChar);
            break;
          }
          else if( cChar=='0' )
          {
            cFillUpChar = '0';
          }
          else if( cChar>'0' && cChar<='9' )
          {
            /* no digit found yet */
            iDigitCnt = 1;
            /* the number started one char before */
            pcNumEnd = pcFmt;
            /* count all digits */
            do
            {
              cChar = *pcFmt;
              if( cChar>='0' && cChar<='9' )
              {
                ++pcFmt;
              }
              else
              {
                break;
              }
            } while(1);

            /* loop over all digits and add them to the */
            uiValue = 0;
            iDigitCnt = 0;
            pcNumCnt = pcFmt;
            while( pcNumCnt>=pcNumEnd )
            {
              --pcNumCnt;
              uiCnt = (*pcNumCnt) & 0x0fU;
              while( uiCnt>0 )
              {
                uiValue += aulUprintfDecTab[iDigitCnt];
                --uiCnt;
              }
              ++iDigitCnt;
            }
            sizMinimumSize = (size_t)uiValue;
          }
          else if( cChar=='x' )
          {
            /* show hexadecimal number */
            ulArgument = va_arg((ptArgument), unsigned long);
            uprintf_hex(ulArgument, sizMinimumSize, cFillUpChar);
            break;
          }
          else if( cChar=='d' )
          {
            /* show decimal number */
            ulArgument = va_arg((ptArgument), unsigned long);
            uprintf_dec(ulArgument, sizMinimumSize, cFillUpChar);
            break;
          }
          else if( cChar=='h' )
          {
            /* show short number */
            ulArgument = va_arg((ptArgument), unsigned int);
            uprintf_dec(ulArgument, sizMinimumSize, cFillUpChar);
            break;
          }
          else if( cChar=='b' )
          {
            /* show binary number */
            ulArgument = va_arg((ptArgument), unsigned long);
            uprintf_bin(ulArgument, sizMinimumSize, cFillUpChar);
            break;
          }
          else if( cChar=='s' )
          {
            /* show string */
            pcArgument = va_arg((ptArgument), const char *);
            uprintf_str(pcArgument, sizMinimumSize, cFillUpChar);
            break;
          }
          else if( cChar=='c' )
          {
            /* show char */
            iArgument = va_arg((ptArgument), int);
            SERIAL_PUT((char)iArgument);
            break;
          }
          else
          {
            SERIAL_PUT('*');
            SERIAL_PUT('*');
            SERIAL_PUT('*');
            break;
          }
        } while( cChar!=0 );
      }
      else if( cChar=='\n' )
      {
        print_linefeed();
      }
      else
      {
        SERIAL_PUT(cChar);
      }
    }
  } while( cChar!=0 );
}

void vuprintln(const char *pcFmt, va_list argp)
{
  vuprintf(pcFmt, argp);
  print_linefeed();
}


void uprintf(const char *pcFmt, ...)
{
  va_list ptArgument;
  va_start(ptArgument, pcFmt);
  vuprintf(pcFmt, ptArgument);
  va_end(ptArgument);
}

void uprintln(const char *pcFmt, ...)
{
  va_list ptArgument;
  va_start(ptArgument, pcFmt);
  vuprintln(pcFmt, ptArgument);
  va_end(ptArgument);
}

int uprint(const char *pszStr)
{
  int iLen = 0;
  char cChar;

  cChar = *(pszStr++);

  while( cChar!='\0' )
  {
    ++iLen;

    if( cChar=='\n' )
    {
      print_linefeed();
    }
    else
    {
      SERIAL_PUT(cChar);
    }
    cChar = *(pszStr++);
  }

  SERIAL_FLUSH();
  return iLen;
}

void hexdump(const unsigned char *pcData, size_t sizData)
{
  const unsigned char *pcDumpCnt;
  const unsigned char *pcDumpEnd;
  unsigned long ulAddressCnt;
  size_t sizChunkCnt;
  size_t sizChunkSize;
  size_t sizBytesLeft;


  /* show a hexdump of the data */
  pcDumpCnt = pcData;
  pcDumpEnd = pcData + sizData;
  ulAddressCnt = 0;
  while( pcDumpCnt<pcDumpEnd )
  {
    /* get number of bytes for the next line */
    sizChunkSize = 16;
    /* trust me, it *is* positive */
    sizBytesLeft = (size_t)(pcDumpEnd-pcDumpCnt);
    if( sizChunkSize>sizBytesLeft )
    {
      sizChunkSize = sizBytesLeft;
    }

    /* start a line in the dump with the address */
    uprintf("%08x: ", ulAddressCnt);
    /* append the data bytes */
    sizChunkCnt = sizChunkSize;
    while( sizChunkCnt!=0 )
    {
      uprintf("%02x ", *pcDumpCnt);
      ++pcDumpCnt;
      --sizChunkCnt;
    }
    ulAddressCnt += sizChunkSize;
    uprintf("\n");
  }
}

