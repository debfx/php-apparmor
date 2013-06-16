/*
  +----------------------------------------------------------------------+
  | php-apprmor                                                          |
  +----------------------------------------------------------------------+
  | Copyright (c) 2013 Felix Geyer <debfx@fobos.de>                      |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Felix Geyer <debfx@fobos.de>                                 |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_APPARMOR_H
#define PHP_APPARMOR_H

extern zend_module_entry apparmor_module_entry;
#define phpext_apparmor_ptr &apparmor_module_entry

#ifdef PHP_WIN32
#	define PHP_APPARMOR_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_APPARMOR_API __attribute__ ((visibility("default")))
#else
#	define PHP_APPARMOR_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(apparmor);
PHP_MSHUTDOWN_FUNCTION(apparmor);
PHP_RINIT_FUNCTION(apparmor);
PHP_RSHUTDOWN_FUNCTION(apparmor);
PHP_MINFO_FUNCTION(apparmor);

ZEND_BEGIN_MODULE_GLOBALS(apparmor)
    char* profile_name;
ZEND_END_MODULE_GLOBALS(apparmor)

#ifdef ZTS
#define APPARMOR_G(v) TSRMG(apparmor_globals_id, zend_apparmor_globals *, v)
#else
#define APPARMOR_G(v) (apparmor_globals.v)
#endif

#endif	/* PHP_APPARMOR_H */
