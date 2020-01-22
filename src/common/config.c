/**
 * @file config.c
 * @brief config parser
 * @author Arnoud Vangrunderbeek
 * @code{.ebp}
 * format: 1TBS
 * rules:
 * @endcode
 */

/* ----------------------------------------------------------------------
 * include section
 * ----------------------------------------------------------------------*/
/* general includes (unix a.o.) - if possible alphabetically ordered */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* project specific includes (Diaser IG) - if possible alphabetically ordered */
#include "log.h"

/* module specific includes (IG App) - if possible alphabetically ordered */

/* component include */
#include "config.h"

/* ----------------------------------------------------------------------
 * internal macro declaration section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * internal type declaration section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * function with internal linkage (static) declaration section
 * ----------------------------------------------------------------------*/

 /**
  * @brief Look for the first alphanum char in the string
  * @param[in] str the string where to search
  * @param[in] n the maximum search size in the string
  * @pre[tested] str must not be null
  * @return the address of the first alnum char. NULL if not found or problem occurred
  * @details
  * If 0, \r or \n is encountered before the first alnum char, NULL is returned.
  */
static const char* APPCFG_StrnFirstAlnum(const char* const str, const size_t n);

 /**
  * @brief Put a null byte in the string at the first occurrence of a char
  * that is not an alnum, nor ':' nor '\' nor '/' nor '.'
  * @param[in] str the string to clean-up
  * @param[in] n the maximum size of the string to clean-up
  * @pre[tested] str must not be null
  * @return error code that shows if pre-conditions were met
  * @attention str must be null terminated before its nth char
  */
static int APPCFG_SanitizeEndOfStrn(char* const str, const size_t n);

/* ----------------------------------------------------------------------
 * variable with internal linkage (static) definition section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * exported variable definition section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * exported function definition section
 * ----------------------------------------------------------------------*/
int APPCFG_GetConfigParamFromFile(
	const char* const filename,
	const char* const param,
	char* const out_value,
	const size_t max_out_len)
{
	int rv;
	FILE* fp = NULL;
	char line[256];
	char needle[256];
	bool found = false;

	if (NULL == filename) {
		APPLOG_Log(__FUNCTION__, LOGLV_ERROR, "Filename is null!!");
		rv = -1;
	}
	else if (NULL == (fp = fopen(filename, "r"))) {
		APPLOG_Log(__FUNCTION__, LOGLV_ERROR, "Failed opening %s: %s!!", filename, strerror(errno));
		rv = -1;
	}
	else if (strlen(param) > sizeof(needle) - 2) {
		APPLOG_Log(__FUNCTION__, LOGLV_ERROR, "param too long : %s!!", param);
		rv = -1;
	}
	else {
		APPLOG_LogDebug(__FUNCTION__, LOGBIT_OCCLI, "%s configuration file successfully opened for %s", filename, param);

		strncpy(needle, param, sizeof(needle) - 2);
		strcat(needle, "=");

		while (fgets(line, sizeof(line) - 1, fp)) {
			char* p_seek;
			if (NULL != (p_seek = strstr(line, needle))) {
				found = true;
				p_seek += strlen(needle);
				p_seek = (char*)APPCFG_StrnFirstAlnum(p_seek, max_out_len);
				if (NULL == p_seek) {
					strcpy(out_value, "");
					rv = 0;
					break;
				}
				else if (strlen(p_seek) > max_out_len) {
					APPLOG_Log(__FUNCTION__, LOGLV_ERROR, "Value string '%s' too long in config file: length is %d - %d allowed", p_seek, strlen(p_seek), max_out_len);
					rv = -1;
					break;
				}
				else {
					strncpy(out_value, p_seek, max_out_len);
					if (0 != APPCFG_SanitizeEndOfStrn(out_value, max_out_len)) {
						APPLOG_Log(__FUNCTION__, LOGLV_ERROR, "Couldn't Sanitize string!");
						rv = -1;
						break;
					}
					else {
						APPLOG_LogDebug(__FUNCTION__, LOGBIT_OCCLI, "%s is %s", param, out_value);
						rv = 0;
						break;
					}
				}
			}
		}

		if (!found) {
			APPLOG_Log(__FUNCTION__, LOGLV_ERROR, "%s parameter not found in config file", param);
			rv = -1;
		}

		fclose(fp);

	}

	return rv;
}

int APPCFG_GetConfigNumericParamFromFile(
	const char* const filename,
	const char* const param,
	void* const out_value,
	const size_t value_length)
{
	static const char* fn = __FUNCTION__;
	char num_str[32];
	//LONG64 num = 0;
	int num = 0;
	int rv;

	num_str[31] = '\x00';

	CHECK_NOT_NULL(out_value)
	else if (0 != (rv = APPCFG_GetConfigParamFromFile(filename, param, num_str, sizeof(num_str) - 1))) {
		APPLOG_Log(fn, LOGLV_ERROR, "Get Param From File failed!");
	}
	else {
		errno = 0;
		num = strtoll(num_str, NULL, 10);
		if (0 != errno) {
			APPLOG_Log(fn, LOGLV_ERROR, "Conversion failed! %s", strerror(errno));
			rv = -1;
		}
		else {
			memcpy(out_value, (void*)&num, value_length);
			APPLOG_LogDebug(__FUNCTION__, LOGBIT_OCCLI, "%s = %lli", param, num);
			rv = 0;
		}
	}

	return rv;
}

/* ----------------------------------------------------------------------
 * functions with internal linkage (static) definition section
 * ----------------------------------------------------------------------*/

static const char* APPCFG_StrnFirstAlnum(const char* const str, const size_t n)
{
	static const char* fn = __FUNCTION__;
	const char* it = str;
	size_t idx;
	bool found = false, end = false;

	if (NULL == str) {
		APPLOG_Log(fn, LOGLV_ERROR, "Null str!");
		it = NULL;
	}
	else {
		for (idx = 0; (idx < n) && !end && !found; idx++) {
			if (isalnum(*it)) {
				found = true;
			}
			else if (('\n' == *it) || ('r' == *it) || (0 == *it)) {
				end = true;
			}
			else {
				it++;
			}
		}

		if (!found) {
			it = NULL;
		}
	}

	return it;
}

static int APPCFG_SanitizeEndOfStrn(char* const str, const size_t n)
{
	static const char* fn = __FUNCTION__;
	char* it = str;
	size_t idx;
	bool found = false;
	int rv;

	if (NULL == str) {
		APPLOG_Log(fn, LOGLV_ERROR, "Null str!");
		rv = -1;
	}
	else {
		for (idx = 0; (idx < n) && !found; idx++) {
			if (!isalnum(*it) && (':' != *it) && ('\\' != *it) && ('/' != *it) && ('.' != *it)) {
				found = true;
				if (0 != *it) {
					*it = 0;
				}
			}
			else {
				it++;
			}
		}

		if (!found) {
			APPLOG_Log(fn, LOGLV_ERROR, "No null found at the end of string!");
			rv = -1;
		}
		else {
			rv = 0;
		}
	}

	return rv;
}