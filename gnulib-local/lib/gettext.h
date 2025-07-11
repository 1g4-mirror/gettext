/* Convenience header for conditional use of GNU <libintl.h>.
   Copyright (C) 1995-2025 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#ifndef _LIBGETTEXT_H
#define _LIBGETTEXT_H 1


/* NLS can be disabled through the configure --disable-nls option
   or through "#define ENABLE NLS 0" before including this file.  */
#if defined ENABLE_NLS && ENABLE_NLS

/* Get declarations of GNU message catalog functions.  */
# include <libintl.h>

/* You can set the DEFAULT_TEXT_DOMAIN macro to specify the domain used by
   the gettext() and ngettext() macros.  This is an alternative to calling
   textdomain(), and is useful for libraries.  */
# ifdef DEFAULT_TEXT_DOMAIN
#  undef gettext
#  define gettext(Msgid) \
     dgettext (DEFAULT_TEXT_DOMAIN, Msgid)
#  undef ngettext
#  define ngettext(Msgid1, Msgid2, N) \
     dngettext (DEFAULT_TEXT_DOMAIN, Msgid1, Msgid2, N)
# endif

#else

/* Solaris /usr/include/locale.h includes /usr/include/libintl.h, which
   chokes if dcgettext is defined as a macro.  So include it now, to make
   later inclusions of <locale.h> a NOP.  We don't include <libintl.h>
   as well because people using "gettext.h" will not include <libintl.h>,
   and also including <libintl.h> would fail on SunOS 4, whereas <locale.h>
   is OK.  */
# if defined(__sun)
#  include <locale.h>
# endif

/* Many header files from the libstdc++ coming with g++ 3.3 or newer include
   <libintl.h>, which chokes if dcgettext is defined as a macro.  So include
   it now, to make later inclusions of <libintl.h> a NOP.  */
# if defined(__cplusplus) && defined(__GNUG__) && (__GNUC__ >= 3)
#  include <cstdlib>
#  if (__GLIBC__ >= 2 && !defined __UCLIBC__) || _GLIBCXX_HAVE_LIBINTL_H
#   include <libintl.h>
#  endif
# endif

/* Disabled NLS.  */
# if defined __GNUC__ && !defined __clang__ && !defined __cplusplus
/* Use inline functions, to avoid warnings
     warning: format not a string literal and no format arguments
   that don't occur with enabled NLS.  */
/* The return type 'const char *' serves the purpose of producing warnings
   for invalid uses of the value returned from these functions.  */
#  if __GNUC__ >= 9
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"
#  endif
__attribute__ ((__always_inline__, __gnu_inline__)) extern inline
#  if !defined(__sun)
const
#  endif
char *
gettext (const char *msgid)
{
  return msgid;
}
__attribute__ ((__always_inline__, __gnu_inline__)) extern inline
#  if !defined(__sun)
const
#  endif
char *
dgettext (const char *domain, const char *msgid)
{
  (void) domain;
  return msgid;
}
__attribute__ ((__always_inline__, __gnu_inline__)) extern inline
#  if !defined(__sun)
const
#  endif
char *
dcgettext (const char *domain, const char *msgid, int category)
{
  (void) domain;
  (void) category;
  return msgid;
}
#  if __GNUC__ >= 9
#   pragma GCC diagnostic pop
#  endif
# else
/* The casts to 'const char *' serve the purpose of producing warnings
   for invalid uses of the value returned from these functions.  */
#  undef gettext
#  define gettext(Msgid) ((const char *) (Msgid))
#  undef dgettext
#  define dgettext(Domainname, Msgid) ((void) (Domainname), gettext (Msgid))
#  undef dcgettext
#  define dcgettext(Domainname, Msgid, Category) \
     ((void) (Category), dgettext (Domainname, Msgid))
# endif
# undef ngettext
# define ngettext(Msgid1, Msgid2, N) \
    ((N) == 1 \
     ? ((void) (Msgid2), (const char *) (Msgid1)) \
     : ((void) (Msgid1), (const char *) (Msgid2)))
# undef dngettext
# define dngettext(Domainname, Msgid1, Msgid2, N) \
    ((void) (Domainname), ngettext (Msgid1, Msgid2, N))
# undef dcngettext
# define dcngettext(Domainname, Msgid1, Msgid2, N, Category) \
    ((void) (Category), dngettext (Domainname, Msgid1, Msgid2, N))
# undef textdomain
# define textdomain(Domainname) ((const char *) (Domainname))
# undef bindtextdomain
# define bindtextdomain(Domainname, Dirname) \
    ((void) (Domainname), (const char *) (Dirname))
# undef bind_textdomain_codeset
# define bind_textdomain_codeset(Domainname, Codeset) \
    ((void) (Domainname), (const char *) (Codeset))

#endif


/* Prefer gnulib's setlocale override over libintl's setlocale override.  */
#ifdef GNULIB_defined_setlocale
# undef setlocale
# define setlocale rpl_setlocale
#endif


/* A pseudo function call that serves as a marker for the automated
   extraction of messages, but does not call gettext().  The run-time
   translation is done at a different place in the code.
   The argument, String, should be a literal string.  Concatenated strings
   and other string expressions won't work.
   The macro's expansion is not parenthesized, so that it is suitable as
   initializer for static 'char[]' or 'const char[]' variables.  */
#define gettext_noop(String) String


/* The separator between msgctxt and msgid in a .mo file.  */
#define GETTEXT_CONTEXT_GLUE "\004"

/* Pseudo function calls, taking a MSGCTXT and a MSGID instead of just a
   MSGID.  MSGCTXT and MSGID must be string literals.  MSGCTXT should be
   short and rarely need to change.
   The letter 'p' stands for 'particular' or 'special'.  */

#include <locale.h> /* for LC_MESSAGES */

#ifdef DEFAULT_TEXT_DOMAIN
# define pgettext(Msgctxt, Msgid) \
   pgettext_aux (DEFAULT_TEXT_DOMAIN, Msgctxt GETTEXT_CONTEXT_GLUE Msgid, Msgid, LC_MESSAGES)
#else
# define pgettext(Msgctxt, Msgid) \
   pgettext_aux (NULL, Msgctxt GETTEXT_CONTEXT_GLUE Msgid, Msgid, LC_MESSAGES)
#endif
#define dpgettext(Domainname, Msgctxt, Msgid) \
  pgettext_aux (Domainname, Msgctxt GETTEXT_CONTEXT_GLUE Msgid, Msgid, LC_MESSAGES)
#define dcpgettext(Domainname, Msgctxt, Msgid, Category) \
  pgettext_aux (Domainname, Msgctxt GETTEXT_CONTEXT_GLUE Msgid, Msgid, Category)
#ifdef DEFAULT_TEXT_DOMAIN
# define npgettext(Msgctxt, Msgid, MsgidPlural, N) \
   npgettext_aux (DEFAULT_TEXT_DOMAIN, Msgctxt GETTEXT_CONTEXT_GLUE Msgid, Msgid, MsgidPlural, N, LC_MESSAGES)
#else
# define npgettext(Msgctxt, Msgid, MsgidPlural, N) \
   npgettext_aux (NULL, Msgctxt GETTEXT_CONTEXT_GLUE Msgid, Msgid, MsgidPlural, N, LC_MESSAGES)
#endif
#define dnpgettext(Domainname, Msgctxt, Msgid, MsgidPlural, N) \
  npgettext_aux (Domainname, Msgctxt GETTEXT_CONTEXT_GLUE Msgid, Msgid, MsgidPlural, N, LC_MESSAGES)
#define dcnpgettext(Domainname, Msgctxt, Msgid, MsgidPlural, N, Category) \
  npgettext_aux (Domainname, Msgctxt GETTEXT_CONTEXT_GLUE Msgid, Msgid, MsgidPlural, N, Category)

#if defined __GNUC__ || defined __clang__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static const char *
pgettext_aux (const char *domain,
              const char *msg_ctxt_id, const char *msgid,
              int category)
{
  const char *translation = dcgettext (domain, msg_ctxt_id, category);
  if (translation == msg_ctxt_id)
    return msgid;
  else
    return translation;
}

#if defined __GNUC__ || defined __clang__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static const char *
npgettext_aux (const char *domain,
               const char *msg_ctxt_id, const char *msgid,
               const char *msgid_plural, unsigned long int n,
               int category)
{
  const char *translation =
    dcngettext (domain, msg_ctxt_id, msgid_plural, n, category);
  if (translation == msg_ctxt_id || translation == msgid_plural)
    return (n == 1 ? msgid : msgid_plural);
  else
    return translation;
}


/* The same thing extended for non-constant arguments.  Here MSGCTXT and MSGID
   can be arbitrary expressions.  But for string literals these macros are
   less efficient than those above.  */

#include <string.h> /* for memcpy */

/* GNULIB_NO_VLA can be defined to disable use of VLAs even if supported.
   This relates to the -Wvla and -Wvla-larger-than warnings, enabled in
   the default GCC many warnings set.  This allows programs to disable use
   of VLAs, which may be unintended, or may be awkward to support portably,
   or may have security implications due to non-deterministic stack usage.  */

#if (!defined GNULIB_NO_VLA \
     && (((__GNUC__ >= 3 || defined __clang__) \
          && !defined __STRICT_ANSI__ && !defined __cplusplus) \
         /* || (__STDC_VERSION__ == 199901L && !defined __HP_cc)
            || (__STDC_VERSION__ >= 201112L && !defined __STDC_NO_VLA__) */ ))
# define _LIBGETTEXT_HAVE_VARIABLE_SIZE_ARRAYS 1
#else
# define _LIBGETTEXT_HAVE_VARIABLE_SIZE_ARRAYS 0
#endif

#if !_LIBGETTEXT_HAVE_VARIABLE_SIZE_ARRAYS
# include <stdlib.h> /* for malloc, free */
#endif

#define pgettext_expr(Msgctxt, Msgid) \
  dcpgettext_expr (NULL, Msgctxt, Msgid, LC_MESSAGES)
#define dpgettext_expr(Domainname, Msgctxt, Msgid) \
  dcpgettext_expr (Domainname, Msgctxt, Msgid, LC_MESSAGES)

#if defined __GNUC__ || defined __clang__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static const char *
dcpgettext_expr (const char *domain,
                 const char *msgctxt, const char *msgid,
                 int category)
{
  size_t msgctxt_len = strlen (msgctxt) + 1;
  size_t msgid_len = strlen (msgid) + 1;
  const char *translation;
#if _LIBGETTEXT_HAVE_VARIABLE_SIZE_ARRAYS
  char msg_ctxt_id[msgctxt_len + msgid_len];
#else
  char buf[1024];
  char *msg_ctxt_id =
    (msgctxt_len + msgid_len <= sizeof (buf)
     ? buf
     : (char *) malloc (msgctxt_len + msgid_len));
  if (msg_ctxt_id != NULL)
#endif
    {
      int found_translation;
      memcpy (msg_ctxt_id, msgctxt, msgctxt_len - 1);
      msg_ctxt_id[msgctxt_len - 1] = '\004';
      memcpy (msg_ctxt_id + msgctxt_len, msgid, msgid_len);
      translation = dcgettext (domain, msg_ctxt_id, category);
      found_translation = (translation != msg_ctxt_id);
#if !_LIBGETTEXT_HAVE_VARIABLE_SIZE_ARRAYS
      if (msg_ctxt_id != buf)
        free (msg_ctxt_id);
#endif
      if (found_translation)
        return translation;
    }
  return msgid;
}

#define npgettext_expr(Msgctxt, Msgid, MsgidPlural, N) \
  dcnpgettext_expr (NULL, Msgctxt, Msgid, MsgidPlural, N, LC_MESSAGES)
#define dnpgettext_expr(Domainname, Msgctxt, Msgid, MsgidPlural, N) \
  dcnpgettext_expr (Domainname, Msgctxt, Msgid, MsgidPlural, N, LC_MESSAGES)

#if defined __GNUC__ || defined __clang__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static const char *
dcnpgettext_expr (const char *domain,
                  const char *msgctxt, const char *msgid,
                  const char *msgid_plural, unsigned long int n,
                  int category)
{
  size_t msgctxt_len = strlen (msgctxt) + 1;
  size_t msgid_len = strlen (msgid) + 1;
  const char *translation;
#if _LIBGETTEXT_HAVE_VARIABLE_SIZE_ARRAYS
  char msg_ctxt_id[msgctxt_len + msgid_len];
#else
  char buf[1024];
  char *msg_ctxt_id =
    (msgctxt_len + msgid_len <= sizeof (buf)
     ? buf
     : (char *) malloc (msgctxt_len + msgid_len));
  if (msg_ctxt_id != NULL)
#endif
    {
      int found_translation;
      memcpy (msg_ctxt_id, msgctxt, msgctxt_len - 1);
      msg_ctxt_id[msgctxt_len - 1] = '\004';
      memcpy (msg_ctxt_id + msgctxt_len, msgid, msgid_len);
      translation = dcngettext (domain, msg_ctxt_id, msgid_plural, n, category);
      found_translation = !(translation == msg_ctxt_id || translation == msgid_plural);
#if !_LIBGETTEXT_HAVE_VARIABLE_SIZE_ARRAYS
      if (msg_ctxt_id != buf)
        free (msg_ctxt_id);
#endif
      if (found_translation)
        return translation;
    }
  return (n == 1 ? msgid : msgid_plural);
}


#endif /* _LIBGETTEXT_H */
