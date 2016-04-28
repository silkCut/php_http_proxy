/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
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
#include "php_anproxy.h"

#define DEFAULT_TRUST_PROXY "127.0.0.1"
#define REMOTE_ADDR   "REMOTE_ADDR"

/* If you declare any globals in php_anproxy.h uncomment this:
 */
ZEND_DECLARE_MODULE_GLOBALS(anproxy)

/* Map the input and standed input
 */
typedef struct {
	char *input;
	char *standed;
}i_to_s;

static i_to_s itos_map[] = {
	{"HTTP_X_FORWARDED_HOST", "X_FORWARDED_HOST"},
	{"HTTP_X_FORWARDED_FOR",  "X_FORWARDED_FOR"},
	{"HTTP_X_FORWARDED_PROTO","X_FORWARDED_PROTO"},
	{"HTTP_X_FORWARDED_PORT", "X_FORWARDED_PORT"},
	{"REMOTE_ADDR", 		"REMOTE_ADDR"},
};
	
/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini*/
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("anproxy.trust_proxies", DEFAULT_TRUST_PROXY, PHP_INI_ALL, OnUpdateString, trust_proxies, zend_anproxy_globals, anproxy_globals)
PHP_INI_END()
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Get About Proxy Info
 */
PHP_FUNCTION(get_proxy_info)
{
	zval** remote_addr;
	char *trust_proxies = DEFAULT_TRUST_PROXY;
	int proxies_len = strlen(trust_proxies);
	long result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &trust_proxies, &proxies_len) == FAILURE ) {
		RETURN_NULL();
	} else {
		if (zend_hash_find(Z_ARRVAL_P(ANPROXY_G(output)), ZEND_STRS(REMOTE_ADDR), /*REMOTE_ADDR, strlen(REMOTE_ADDR) +1,*/ (void **)&remote_addr) == FAILURE) {
			RETURN_NULL();
		}
		if (result = strcmp(Z_STRVAL_PP(remote_addr), trust_proxies) != 0) {
			RETURN_NULL();
		}
		RETVAL_ZVAL(ANPROXY_G(output), 1, 0);
		return;
	}
}

/* Get varies from CG
*/
static zval** ap_ori_input(char *name, int name_len)
{
	zval **tmp;
	int result = zend_is_auto_global(name, name_len TSRMLS_CC);
	if (result == 1)  {
		if (zend_hash_find(&EG(symbol_table), name, name_len + 1, (void **)&tmp) != FAILURE){
			zend_hash_internal_pointer_reset(Z_ARRVAL_PP(tmp));
			return tmp;
		}
	}
	return NULL;
}

static int php_sample_print_zval(zval **val TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key);

/* }}} */
static void set_var_to_gal()
{
	int i = 0;
	int len = sizeof(itos_map)/sizeof(i_to_s);
	zval *val;
	for(i = 0; i < len; i++){
		char *input = itos_map[i].input;
		char *output = itos_map[i].standed;
		
		if (zend_hash_find(ANPROXY_G(server), input, strlen(input) + 1, (void **)&val) == SUCCESS)
		{
			zend_hash_update(Z_ARRVAL_P(ANPROXY_G(output)), output, strlen(output) + 1, val, sizeof(zval), NULL);
		} 
	} 	
		//zend_hash_apply_with_arguments((ANPROXY_G(server)) TSRMLS_DC, (apply_func_args_t)php_sample_print_zval, 2);
	   //zend_hash_apply_with_arguments(Z_ARRVAL_P(ANPROXY_G(output)) TSRMLS_DC, (apply_func_args_t)php_sample_print_zval, 2);
}

static int php_sample_print_zval(zval **val TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
{
		//重新copy一个zval，防止破坏原数据
		zval tmpcopy = **val;
		zval_copy_ctor(&tmpcopy);
		           
		//转换为字符串
		INIT_PZVAL(&tmpcopy);
		convert_to_string(&tmpcopy);
	
		//开始输出

		php_printf("The key is: ");
		PHPWRITE(hash_key->arKey,hash_key->nKeyLength);
		php_printf("\n<br\>");

		php_printf("The value is: ");
		PHPWRITE(Z_STRVAL(tmpcopy), Z_STRLEN(tmpcopy));
		php_printf("\n<br\>");
		             	                
		//毁尸灭迹
		zval_dtor(&tmpcopy);
		                      	                            
		//返回，继续遍历下一个～
		return ZEND_HASH_APPLY_KEEP;
}	

/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ php_anproxy_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_anproxy_init_globals(zend_anproxy_globals *anproxy_globals)
{
	anproxy_globals->global_value = 0;
	anproxy_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION Get All Input Info
 */
PHP_MINIT_FUNCTION(anproxy)
{
	REGISTER_INI_ENTRIES();

	//init output info
	/*
	ALLOC_ZVAL(ANPROXY_G(output));
	array_init(ANPROXY_G(output));
	INIT_PZVAL(ANPROXY_G(output));
	*/

	/*
	zval **input_server;
	input_server = ap_ori_input(ZEND_STRL("_SERVER"));
	//zval* input_env = ap_ori_input(ZEND_STRL("_ENV"));
	//php_autoglobal_merge(Z_ARRVAL_P(input_server), Z_ARRVAL_P(input_env));
	ANPROXY_G(server) = Z_ARRVAL_PP(input_server);
	set_var_to_gal();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(anproxy)
{
	/* uncomment this line if you have INI entries*/
	UNREGISTER_INI_ENTRIES();
	//zval_ptr_dtor(&ANPROXY_G(output));
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(anproxy)
{
	zval **input_server;
	ALLOC_ZVAL(ANPROXY_G(output));
	array_init(ANPROXY_G(output));
	INIT_PZVAL(ANPROXY_G(output));
	input_server = ap_ori_input(ZEND_STRL("_SERVER"));
	//zval* input_env = ap_ori_input(ZEND_STRL("_ENV"));
	//php_autoglobal_merge(Z_ARRVAL_P(input_server), Z_ARRVAL_P(input_env));
	ANPROXY_G(server) = Z_ARRVAL_PP(input_server);
	set_var_to_gal();
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(anproxy)
{
	zval_ptr_dtor(&ANPROXY_G(output));
	return SUCCESS;
}

/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(anproxy)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "anproxy support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

#if (PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 0)
    ZEND_BEGIN_ARG_INFO_EX(return_by_ref_arginfo, 0, 1, 0)
	ZEND_END_ARG_INFO ()
#endif /* PHP >= 5.1.0 */

/* {{{ anproxy_functions[]
 *
 * Every user visible function must have an entry in anproxy_functions[].
 */
const zend_function_entry anproxy_functions[] = {
	PHP_FE(get_proxy_info, return_by_ref_arginfo)		
	PHP_FE_END	/* Must be the last line in anproxy_functions[] */
};
/* }}} */

/* {{{ anproxy_module_entry
 */
zend_module_entry anproxy_module_entry = {
	STANDARD_MODULE_HEADER,
	"anproxy",
	anproxy_functions,
	PHP_MINIT(anproxy),
	PHP_MSHUTDOWN(anproxy),
	PHP_RINIT(anproxy),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(anproxy),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(anproxy),
	PHP_ANPROXY_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ANPROXY
ZEND_GET_MODULE(anproxy)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
