dnl config.m4 for extension apparmor

PHP_ARG_WITH(apparmor, for apparmor support,
[  --with-apparmor             Include apparmor support])

if test "$PHP_APPARMOR" != "no"; then
  # --with-apparmor -> check with-path
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/sys/apparmor.h"
  if test -r $PHP_APPARMOR/$SEARCH_FOR; then # path given as parameter
    APPARMOR_DIR=$PHP_APPARMOR
  else # search default path list
    AC_MSG_CHECKING([for apparmor files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        APPARMOR_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  dnl
  if test -z "$APPARMOR_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the apparmor distribution])
  fi

  # --with-apparmor -> add include path
  PHP_ADD_INCLUDE($APPARMOR_DIR/include)

  # --with-apparmor -> check for lib and symbol presence
  LIBNAME=apparmor
  LIBSYMBOL=aa_change_profile

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $APPARMOR_DIR/lib, APPARMOR_SHARED_LIBADD)
    AC_DEFINE(HAVE_APPARMORLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong apparmor lib version or lib not found])
  ],[
    -L$APPARMOR_DIR/lib -lm
  ])
  dnl
  PHP_SUBST(APPARMOR_SHARED_LIBADD)

  PHP_NEW_EXTENSION(apparmor, apparmor.c, $ext_shared)
fi
