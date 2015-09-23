/*
   http://yukihiro.nakadaira.googlepages.com/#win_iconv
*/
#ifndef WIN_ICONV_H
#define WIN_ICONV_H 1

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef void* iconv_t;
  iconv_t iconv_open(const char *tocode, const char *fromcode);
  int iconv_close(iconv_t cd);
  size_t iconv(iconv_t cd, const char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft);

#ifdef __cplusplus
}
#endif

#endif //WIN_ICONV_H
