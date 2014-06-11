dnl $Id$
dnl config.m4 for extension arrayutils

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(arrayutils, for arrayutils support,
Make sure that the comment is aligned:
[  --with-arrayutils             Include arrayutils support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(arrayutils, whether to enable arrayutils support,
dnl Make sure that the comment is aligned:
dnl [  --enable-arrayutils           Enable arrayutils support])

if test "$PHP_ARRAYUTILS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-arrayutils -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/arrayutils.h"  # you most likely want to change this
  dnl if test -r $PHP_ARRAYUTILS/$SEARCH_FOR; then # path given as parameter
  dnl   ARRAYUTILS_DIR=$PHP_ARRAYUTILS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for arrayutils files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ARRAYUTILS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ARRAYUTILS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the arrayutils distribution])
  dnl fi

  dnl # --with-arrayutils -> add include path
  dnl PHP_ADD_INCLUDE($ARRAYUTILS_DIR/include)

  dnl # --with-arrayutils -> check for lib and symbol presence
  dnl LIBNAME=arrayutils # you may want to change this
  dnl LIBSYMBOL=arrayutils # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ARRAYUTILS_DIR/lib, ARRAYUTILS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ARRAYUTILSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong arrayutils lib version or lib not found])
  dnl ],[
  dnl   -L$ARRAYUTILS_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ARRAYUTILS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(arrayutils, arrayutils.c, $ext_shared)
fi
