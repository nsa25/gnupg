dnl Check for libcurl and dependencies
dnl Copyright (C) 2004 Free Software Foundation, Inc.
dnl
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.
dnl
dnl Defines HAVE_LIBCURL to 1 if a working curl setup is found, and sets
dnl @LIBCURL@ and @LIBCURL_INCLUDES@ to the necessary values.
dnl $have_libcurl is set to yes or no so other things in configure can
dnl make use of it.

AC_DEFUN([GNUPG_CHECK_LIBCURL],
[
  AC_ARG_WITH(libcurl,
     AC_HELP_STRING([--with-libcurl=DIR],[look for the curl library in DIR]),
     [_do_libcurl=$withval],[_do_libcurl=no])

  if test "$_do_libcurl" != "no" ; then
     if test -d "$withval" ; then
        CPPFLAGS="${CPPFLAGS} -I$withval/include"
        LDFLAGS="${LDFLAGS} -L$withval/lib"
     fi

     AC_MSG_CHECKING([for curl-config])

     if eval curl-config --version 2>/dev/null >/dev/null; then
        if test x"$LIBCURL_INCLUDES" = "x" ; then
           LIBCURL_INCLUDES=`curl-config --cflags`
        fi
        if test x"$LIBCURL" = "x" ; then
           LIBCURL=`curl-config --libs`

 	   # This is so silly, but Apple actually has a bug in their
	   # curl-config script!

           case "${host}" in
              powerpc-apple-darwin*)
                 LIBCURL=`echo $LIBCURL | sed -e 's|-arch i386||g'`
   	      ;;
           esac
        fi
        _have_config=yes
     else
        _have_config=no
     fi

     AC_MSG_RESULT([$_have_config])

     # we didn't find curl-config, so let's see if the user-supplied
     # link line (or failing that, "-lcurl") is enough.

     LIBCURL=${LIBCURL-"-lcurl"}

     AC_MSG_CHECKING([whether libcurl is usable])

     _libcurl_save_cppflags=$CPPFLAGS
     CPPFLAGS="$CPPFLAGS $LIBCURL_INCLUDES"
     _libcurl_save_ldflags=$LDFLAGS
     LDFLAGS="$LDFLAGS $LIBCURL"

     AC_LINK_IFELSE(AC_LANG_PROGRAM([#include <curl/curl.h>],[
int x;
x=CURL_ERROR_SIZE;
curl_easy_setopt(NULL,CURLOPT_URL,NULL);
x=CURLOPT_WRITEFUNCTION;
x=CURLOPT_FILE;
/* x=CURLOPT_WRITEDATA; */
x=CURLOPT_ERRORBUFFER;
x=CURLOPT_STDERR;
x=CURLOPT_VERBOSE;
]),have_libcurl=yes,have_libcurl=no)

     CPPFLAGS=$_libcurl_save_cppflags
     LDFLAGS=$_libcurl_save_ldflags

     AC_MSG_RESULT([$have_libcurl])

     if test $have_libcurl = yes ; then
        AC_DEFINE(HAVE_LIBCURL,1,
          [Define to 1 if you have a fully functional curl library.])
        AC_SUBST(LIBCURL_INCLUDES)
        AC_SUBST(LIBCURL)
     fi

     unset _do_libcurl
     unset _have_config
     unset _libcurl_save_cppflags
     unset _libcurl_save_ldflags
  fi
])dnl
