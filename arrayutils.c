/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_arrayutils.h"

/* If you declare any globals in php_arrayutils.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(arrayutils)
*/

/* True global resources - no need for thread safety here */
static int le_arrayutils;

/* {{{ arrayutils_functions[]
 *
 * Every user visible function must have an entry in arrayutils_functions[].
 */
const zend_function_entry arrayutils_functions[] = {
	PHP_FE(array_map_with_key, NULL)
	PHP_FE(array_filter_with_key, NULL)
	PHP_FE(array_take_while, NULL)
	PHP_FE(array_drop_while, NULL)
    PHP_FE(array_get_or_else, NULL)
	PHP_FE_END	/* Must be the last line in arrayutils_functions[] */
};
/* }}} */

/* {{{ arrayutils_module_entry
 */
zend_module_entry arrayutils_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"arrayutils",
	arrayutils_functions,
	PHP_MINIT(arrayutils),
	PHP_MSHUTDOWN(arrayutils),
	PHP_RINIT(arrayutils),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(arrayutils),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(arrayutils),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_ARRAYUTILS_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ARRAYUTILS
ZEND_GET_MODULE(arrayutils)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("arrayutils.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_arrayutils_globals, arrayutils_globals)
    STD_PHP_INI_ENTRY("arrayutils.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_arrayutils_globals, arrayutils_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_arrayutils_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_arrayutils_init_globals(zend_arrayutils_globals *arrayutils_globals)
{
	arrayutils_globals->global_value = 0;
	arrayutils_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(arrayutils)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(arrayutils)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(arrayutils)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(arrayutils)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(arrayutils)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "arrayutils support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

PHP_FUNCTION(array_map_with_key)
{
    zval *array;

    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;
    zval **params[2];
    zval **first_arg = NULL;
    zval *second_arg = NULL;
    zval *retval_ptr = NULL;

    char *string_key;
    uint string_key_len;
    ulong num_key;
    HashPosition pos;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "af", &array, &fci, &fci_cache) == FAILURE) {
        return;
    }

    array_init(return_value);
    MAKE_STD_ZVAL(second_arg);

    fci.param_count = 2;
    fci.no_separation = 0;

    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
        zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **) &first_arg, &pos) == SUCCESS;
        zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos)
    ) {

        zend_hash_get_current_key_zval_ex(Z_ARRVAL_P(array), second_arg, &pos);

        params[0] = first_arg;
        params[1] = &second_arg;

        fci.param_count = 2;
        fci.params = params;
        fci.no_separation = 0;
        fci.retval_ptr_ptr = &retval_ptr;

        if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS && retval_ptr) {

            zval_add_ref(&retval_ptr);

            switch (zend_hash_get_current_key_ex(Z_ARRVAL_P(array), &string_key, &string_key_len, &num_key, 0, &pos)) {
                case HASH_KEY_IS_STRING:
                    zend_hash_update(Z_ARRVAL_P(return_value), string_key, string_key_len, (void **) &retval_ptr, sizeof(zval *), NULL);
                    break;

                case HASH_KEY_IS_LONG:
                    zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, (void **) &retval_ptr, sizeof(zval *), NULL);
                    break;
            }

            zval_ptr_dtor(&retval_ptr);

        }

    }
}

PHP_FUNCTION(array_filter_with_key) {

    zval *array;

    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;
    zval **params[2];
    zval **first_arg = NULL;
    zval *second_arg = NULL;
    zval *retval_ptr = NULL;

    char *string_key;
    uint string_key_len;
    ulong num_key;
    HashPosition pos;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "af", &array, &fci, &fci_cache) == FAILURE) {
        return;
    }

    array_init(return_value);
    MAKE_STD_ZVAL(second_arg);

    fci.param_count = 2;
    fci.params = params;
    fci.retval_ptr_ptr = &retval_ptr;
    fci.no_separation = 0;

    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
        zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **) &first_arg, &pos) == SUCCESS;
        zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos)
    ) {

        zend_hash_get_current_key_zval_ex(Z_ARRVAL_P(array), second_arg, &pos);

        params[0] = first_arg;
        params[1] = &second_arg;
        fci.params = params;

        if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS && retval_ptr) {

            if (!zend_is_true(retval_ptr)) {
                zval_ptr_dtor(&retval_ptr);
                continue;
            } else {
                zval_ptr_dtor(&retval_ptr);
            }

        }

        switch (zend_hash_get_current_key_ex(Z_ARRVAL_P(array), &string_key, &string_key_len, &num_key, 0, &pos)) {
            case HASH_KEY_IS_STRING:
                zend_hash_update(Z_ARRVAL_P(return_value), string_key, string_key_len, first_arg, sizeof(zval *), NULL);
                break;

            case HASH_KEY_IS_LONG:
                zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, first_arg, sizeof(zval *), NULL);
                break;
        }

    }

}

PHP_FUNCTION(array_take_while) {

    zval *array;

    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;
    zval **params[2];
    zval **first_arg = NULL;
    zval *second_arg = NULL;
    zval *retval_ptr = NULL;

    char *string_key;
    uint string_key_len;
    ulong num_key;
    HashPosition pos;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "af", &array, &fci, &fci_cache) == FAILURE) {
        return;
    }

    array_init(return_value);
    MAKE_STD_ZVAL(second_arg);

    fci.param_count = 2;
    fci.params = params;
    fci.retval_ptr_ptr = &retval_ptr;
    fci.no_separation = 0;

    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
        zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **) &first_arg, &pos) == SUCCESS;
        zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos)
    ) {

        zend_hash_get_current_key_zval_ex(Z_ARRVAL_P(array), second_arg, &pos);

        params[0] = first_arg;
        params[1] = &second_arg;
        fci.params = params;

        if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS && retval_ptr) {

            if (zend_is_true(retval_ptr)) {
                zval_ptr_dtor(&retval_ptr);
                break;
            } else {
                zval_ptr_dtor(&retval_ptr);
            }

        }

        zval_add_ref(first_arg);

        switch (zend_hash_get_current_key_ex(Z_ARRVAL_P(array), &string_key, &string_key_len, &num_key, 0, &pos)) {
            case HASH_KEY_IS_STRING:
                zend_hash_update(Z_ARRVAL_P(return_value), string_key, string_key_len, first_arg, sizeof(zval *), NULL);
                
                break;

            case HASH_KEY_IS_LONG:
                zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, first_arg, sizeof(zval *), NULL);
                break;
        }

    }

}

PHP_FUNCTION(array_drop_while) {

    zval *array;

    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;
    zval **params[2];
    zval **first_arg = NULL;
    zval *second_arg = NULL;
    zval *retval_ptr = NULL;

    char *string_key;
    uint string_key_len;
    ulong num_key;
    HashPosition pos;

    zend_bool checked = false;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "af", &array, &fci, &fci_cache) == FAILURE) {
        return;
    }

    array_init(return_value);
    MAKE_STD_ZVAL(second_arg);

    fci.param_count = 2;
    fci.params = params;
    fci.retval_ptr_ptr = &retval_ptr;
    fci.no_separation = 0;

    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
        zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **) &first_arg, &pos) == SUCCESS;
        zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos)
    ) {

        zend_hash_get_current_key_zval_ex(Z_ARRVAL_P(array), second_arg, &pos);

        params[0] = first_arg;
        params[1] = &second_arg;
        fci.params = params;

        if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS
            && retval_ptr
            && (checked || zend_is_true(retval_ptr))) {

            checked = true;
            zval_add_ref(first_arg);

            switch (zend_hash_get_current_key_ex(Z_ARRVAL_P(array), &string_key, &string_key_len, &num_key, 0, &pos)) {
                case HASH_KEY_IS_STRING:
                    zend_hash_update(Z_ARRVAL_P(return_value), string_key, string_key_len, first_arg, sizeof(zval *), NULL);
                    break;

                case HASH_KEY_IS_LONG:
                    zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, first_arg, sizeof(zval *), NULL);
                    break;
            }

        }

    }

    if (retval_ptr) {
        zval_ptr_dtor(&retval_ptr);
    }
}

PHP_FUNCTION(array_get_or_else)
{
    zval *array, *offset, **entry, **default_value;
    long index = 0;
    char *key;
    int key_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "azZ", &array, &offset, &default_value) == FAILURE) {
        return;
    }
 
    switch (Z_TYPE_P(offset)) {
        case IS_NULL:
            index = 0;
            break;
        case IS_DOUBLE:
            index = (long) Z_DVAL_P(offset);
            break;
        case IS_BOOL:
        case IS_LONG:
        case IS_RESOURCE:
            index = Z_LVAL_P(offset);
            break;
        case IS_STRING:
            key = Z_STRVAL_P(offset);
            key_len = Z_STRLEN_P(offset);
            break;
        default:
            key = "Unknown";
            key_len = sizeof("Unknown") - 1;
    }

    if (!key && zend_hash_index_find(Z_ARRVAL_P(array), index, (void **) &entry) == FAILURE) {
        RETURN_ZVAL(*default_value, 1, 0);
    } else if (key && zend_hash_find(Z_ARRVAL_P(array), key, key_len + 1, (void **) &entry) == FAILURE) {
        RETURN_ZVAL(*default_value, 1, 0);
    } else 
 
    RETURN_ZVAL(*entry, 1, 0);
}

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
