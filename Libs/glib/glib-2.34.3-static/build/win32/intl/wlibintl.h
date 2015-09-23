/* Message catalogs for internationalization. "wchar_t" support.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
   USA.  */

#ifndef WLIBINTL_H
#define WLIBINTL_H	1

#ifdef __cplusplus
extern "C"
{
#endif

/* wchar_t support */
#ifdef _INTL_REDIRECT_INLINE
#define WCHAR_LIBINTL_STATIC_INLINE static inline
#elif defined(_MSC_VER)
#define WCHAR_LIBINTL_STATIC_INLINE static _inline
#else
#define WCHAR_LIBINTL_STATIC_INLINE static
#endif

#define wgettext(msgid) wgettext_(msgid, L##msgid)
#define dwgettext(domainname, msgid) dwgettext_(domainname, msgid, L##msgid)
#define dcwgettext(domainname, msgid, category) dwgettext_(domainname, msgid, category, L##msgid)
#define nwgettext(msgid1, msgid2, n) nwgettext_(msgid1, msgid2, n, L##msgid1,  L##msgid2)
#define dnwgettext(domainname, msgid1, msgid2, n) dnwgettext_(domainname, msgid1, msgid2, n, L##msgid1,  L##msgid2)
#define dcnwgettext(domainname, msgid1, msgid2, n, category) dnwgettext_(domainname, msgid1, msgid2, n, L##msgid1,  L##msgid2)

WCHAR_LIBINTL_STATIC_INLINE 
const wchar_t *wgettext_ (const char *msgid, const wchar_t *wmsgid)
{
  const char *translated = gettext (msgid);
  return translated != msgid ? (const wchar_t *)translated : wmsgid;
}

WCHAR_LIBINTL_STATIC_INLINE 
const wchar_t *dwgettext_ (const char *domainname, const char *msgid, const wchar_t *wmsgid)
{
  const char *translated = dgettext (domainname, msgid);
  return translated != msgid ? (const wchar_t *)translated : wmsgid;
}

WCHAR_LIBINTL_STATIC_INLINE 
const wchar_t *dcwgettext_ (const char *domainname, const char *msgid, int category,  const wchar_t *wmsgid)
{
  const char *translated = dcgettext (domainname, msgid, category);
  return translated != msgid ? (const wchar_t *)translated : wmsgid;
}

WCHAR_LIBINTL_STATIC_INLINE 
const wchar_t *nwgettext_ (const char *msgid1, const char *msgid2, unsigned long n, const wchar_t *wmsgid1, const wchar_t *wmsgid2)
{
  const char *translated = ngettext (msgid1, msgid2, n);
  return translated == msgid1 ? wmsgid1 : (translated == msgid2 ? wmsgid2 : (const wchar_t *)translated);
}

WCHAR_LIBINTL_STATIC_INLINE 
const wchar_t *dnwgettext_ (const char *domainname, const char *msgid1, const char *msgid2, unsigned long n, const wchar_t *wmsgid1, const wchar_t *wmsgid2)
{
  const char *translated = dngettext (domainname, msgid1, msgid2, n);
  return translated == msgid1 ? wmsgid1 : (translated == msgid2 ? wmsgid2 : (const wchar_t *)translated);
}

WCHAR_LIBINTL_STATIC_INLINE 
const wchar_t *dcnwgettext_ (const char *domainname, const char *msgid1, const char *msgid2, unsigned long n, const wchar_t *wmsgid1, const wchar_t *wmsgid2, int category)
{
  const char *translated = dcngettext (domainname, msgid1, msgid2, n, category);
  return translated == msgid1 ? wmsgid1 : (translated == msgid2 ? wmsgid2 : (const wchar_t *)translated);
}

/* MS Windows Wide character support.  */
#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)) \
    && !defined(__CYGWIN__) && (defined(UNICODE) || defined(_UNICODE))
extern const wchar_t *wbindtextdomain (const char *domainname, const wchar_t *dirname16);
#endif

#undef WCHAR_LIBINTL_STATIC_INLINE 

#ifdef __cplusplus
}
#endif

#endif /* wlibintl.h */

