dnl $Id$
dnl config.m4 for extension anprox

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(anprox, for anprox support,
dnl Make sure that the comment is aligned:
dnl [  --with-anprox             Include anprox support])

dnl Otherwise use enable:

 PHP_ARG_ENABLE(anproxy, whether to enable anprox support,
 Make sure that the comment is aligned:
 [  --enable-anproxy           Enable anprox support])

if test "$PHP_ANPROX" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-anprox -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/anprox.h"  # you most likely want to change this
  dnl if test -r $PHP_ANPROX/$SEARCH_FOR; then # path given as parameter
  dnl   ANPROX_DIR=$PHP_ANPROX
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for anprox files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ANPROX_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ANPROX_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the anprox distribution])
  dnl fi

  dnl # --with-anprox -> add include path
  dnl PHP_ADD_INCLUDE($ANPROX_DIR/include)

  dnl # --with-anprox -> check for lib and symbol presence
  dnl LIBNAME=anprox # you may want to change this
  dnl LIBSYMBOL=anprox # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ANPROX_DIR/$PHP_LIBDIR, ANPROX_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ANPROXLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong anprox lib version or lib not found])
  dnl ],[
  dnl   -L$ANPROX_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ANPROX_SHARED_LIBADD)

  PHP_NEW_EXTENSION(anproxy, anproxy.c, $ext_shared)
fi
