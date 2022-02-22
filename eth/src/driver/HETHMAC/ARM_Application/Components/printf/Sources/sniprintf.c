/****************************************************************************************
*  A simple implementation of sniprintf() with limited format support and small footprint
****************************************************************************************/

#include "sniprintf.h"

#include <stddef.h>
#include <string.h>

static int snprintf_hex(char** ppBuf, size_t sizBuf, unsigned long ulValue, size_t sizMinimum, char cFillUpChar)
{
  int fLeadingDigitWasPrinted;
  size_t sizCnt;
  unsigned int uiDigit;
  int iOutLen;
  char* pBuf = *ppBuf;

  /* number of output chars */
  iOutLen = 0;

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
        if( ++iOutLen<sizBuf )
        {
          *pBuf++ = cFillUpChar;
        }
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
      if( ++iOutLen<sizBuf )
      {
        *pBuf++ = (char)uiDigit;
      }

      /* now the leading digit has been printed */
      fLeadingDigitWasPrinted = 1;
    }

    /* move to next digit */
    ulValue <<= 4;
  } while( sizCnt>0 );

  *ppBuf = pBuf;
  return iOutLen;
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

static int snprintf_dec(char** ppBuf, size_t sizBuf, unsigned long ulValue, size_t sizMinimum, char cFillUpChar)
{
  int fLeadingDigitWasPrinted;
  size_t sizCnt;
  unsigned int uiDigit;
  int iOutLen;
  char* pBuf = *ppBuf;

  /* number of output chars */
  iOutLen = 0;

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
        if( ++iOutLen<sizBuf )
        {
          *pBuf++ = cFillUpChar;
        }
      }
    }
    else
    {
      /* print the digit */
      uiDigit |= '0';
      if( ++iOutLen<sizBuf )
      {
        *pBuf++ = (char)uiDigit;
      }

      /* now the leading digit has been printed */
      fLeadingDigitWasPrinted = 1;
    }
  } while( sizCnt>0 );

  *ppBuf = pBuf;
  return iOutLen;
}


static int snprintf_bin(char** ppBuf, size_t sizBuf, unsigned long ulValue, size_t sizMinimum, char cFillUpChar)
{
  int fLeadingDigitWasPrinted;
  size_t sizCnt;
  unsigned int uiDigit;
  int iOutLen;
  char* pBuf = *ppBuf;

  /* number of output chars */
  iOutLen = 0;

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
        if( ++iOutLen<sizBuf )
        {
          *pBuf++ = cFillUpChar;
        }
      }
    }
    else
    {
      /* print the digit */
      uiDigit |= '0';
      if( ++iOutLen<sizBuf )
      {
        *pBuf++ = (char)uiDigit;
      }

      /* now the leading digit has been printed */
      fLeadingDigitWasPrinted = 1;
    }

    /* move to next digit */
    ulValue <<= 1;
  } while( sizCnt>0 );

  *ppBuf = pBuf;
  return iOutLen;
}


static int snprintf_str(char** ppBuf, size_t sizBuf, const char *pcString, size_t sizMinimum, char cFillUpChar)
{
  size_t sizString;
  size_t sizCnt;
  const char *pcCnt;
  char cChar;
  int iOutLen;
  char* pBuf = *ppBuf;

  /* number of output chars */
  iOutLen = 0;

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
    if( ++iOutLen<sizBuf )
    {
      *pBuf++ = cFillUpChar;
    }
    ++sizCnt;
  }

  /* print the string */
  pcCnt = pcString;
  do
  {
    cChar = *(pcCnt++);
    if( cChar!='\0' )
    {
      if( ++iOutLen<sizBuf )
      {
        *pBuf++ = cChar;
      }
    }
  } while( cChar!='\0' );

  *ppBuf = pBuf;
  return iOutLen;
}

int vsniprintf(char* pBuf, size_t sizBuf, const char *pcFmt, va_list ptArgument)
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
  int iOutLen;

  /* number of output chars */
  iOutLen = 0;

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
            if( ++iOutLen<sizBuf )
            {
              *pBuf++ = cChar;
            }
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
            iOutLen += snprintf_hex(&pBuf, ( iOutLen < sizBuf ) ? (sizBuf - iOutLen) : 0, ulArgument, sizMinimumSize, cFillUpChar);
            break;
          }
          else if( cChar=='d' )
          {
            /* show decimal number */
            ulArgument = va_arg((ptArgument), unsigned long);
            iOutLen += snprintf_dec(&pBuf, ( iOutLen < sizBuf ) ? (sizBuf - iOutLen) : 0, ulArgument, sizMinimumSize, cFillUpChar);
            break;
          }
          else if( cChar=='b' )
          {
            /* show binary number */
            ulArgument = va_arg((ptArgument), unsigned long);
            iOutLen += snprintf_bin(&pBuf, ( iOutLen < sizBuf ) ? (sizBuf - iOutLen) : 0, ulArgument, sizMinimumSize, cFillUpChar);
            break;
          }
          else if( cChar=='s' )
          {
            /* show string */
            pcArgument = va_arg((ptArgument), const char *);
            iOutLen += snprintf_str(&pBuf, ( iOutLen < sizBuf ) ? (sizBuf - iOutLen) : 0, pcArgument, sizMinimumSize, cFillUpChar);
            break;
          }
          else if( cChar=='c' )
          {
            /* show char */
            iArgument = va_arg((ptArgument), int);
            if( ++iOutLen<sizBuf )
            {
              *pBuf++ = (char)iArgument;
            }
            break;
          }
          else
          {
            if( ++iOutLen<sizBuf )
            {
              *pBuf++ = '*';
            }
            if( ++iOutLen<sizBuf )
            {
              *pBuf++ = '*';
            }
            if( ++iOutLen<sizBuf )
            {
              *pBuf++ = '*';
            }
            break;
          }
        } while( cChar!=0 );
      }
      else
      {
        if( ++iOutLen<sizBuf )
        {
          *pBuf++ = cChar;
        }
      }
    }
  } while( cChar!=0 );

  /* zero-termination */
  *pBuf++ = '\0';

  return iOutLen;
}


int sniprintf(char* pBuf, size_t sizBuf, const char *pcFmt, ...)
{
  va_list ptArgument;
  int iOutLen;

  va_start(ptArgument, pcFmt);
  iOutLen = vsniprintf(pBuf, sizBuf, pcFmt, ptArgument);
  va_end(ptArgument);
  return iOutLen;
}
