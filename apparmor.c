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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_apparmor.h"

ZEND_DECLARE_MODULE_GLOBALS(apparmor)

/* True global resources - no need for thread safety here */
static int le_apparmor;

/* {{{ apparmor_functions[]
 *
 * Every user visible function must have an entry in apparmor_functions[].
 */
const zend_function_entry apparmor_functions[] = {
    PHP_FE_END    /* Must be the last line in apparmor_functions[] */
};
/* }}} */

/* {{{ apparmor_module_entry
 */
zend_module_entry apparmor_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "apparmor",
    apparmor_functions,
    PHP_MINIT(apparmor),
    PHP_MSHUTDOWN(apparmor),
    PHP_RINIT(apparmor),        /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(apparmor),    /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(apparmor),
#if ZEND_MODULE_API_NO >= 20010901
    "0.1",
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_APPARMOR
ZEND_GET_MODULE(apparmor)
#endif

static PHP_INI_MH(ChangeProfile)
{
    APPARMOR_G(profile_name) = new_value;

    if (new_value) {
        if (aa_change_profile(new_value) < 0) {
            php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to change to AppArmor profile \"%s\": %s.", new_value, strerror(errno));
            return FAILURE;
        }
    }

    return SUCCESS;
}

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("apparmor.profile", NULL, PHP_INI_SYSTEM, ChangeProfile, profile_name, zend_apparmor_globals, apparmor_globals)
PHP_INI_END()
/* }}} */

/* {{{ php_apparmor_init_globals
 */
static void php_apparmor_init_globals(zend_apparmor_globals *apparmor_globals)
{
    apparmor_globals->profile_name = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(apparmor)
{
    ZEND_INIT_MODULE_GLOBALS(apparmor, php_apparmor_init_globals, NULL);
    REGISTER_INI_ENTRIES();

    /*const char* profile_name = APPARMOR_G(profile_name);
    if (profile_name) {

    }*/

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(apparmor)
{
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(apparmor)
{
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(apparmor)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(apparmor)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "apparmor support", "enabled");
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}
/* }}} */
