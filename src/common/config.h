#if !defined (CONFIG_H_INCLUDE)
#define CONFIG_H_INCLUDE
/**
 * @file config.h
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

/* project specific includes (Diaser IG) - if possible alphabetically ordered */

/* module specific includes (skeleton) - if possible alphabetically ordered */

/* component include */
#include "config_t.h"

/* ----------------------------------------------------------------------
 * function declaration section
 * ----------------------------------------------------------------------*/

 /**
  * @brief Check that pointer x is not null
  * @param[in] x the pointer to check
  * @pre[untested] return neg value for failure
  */
#define CHECK_NOT_NULL(x) if (NULL == (x)){          \
		rv = -1;									 \
	}

/**
 * @brief Parse config file for getting the configuration parameter
 * @param[in] filename the filename where to find the param and its value
 * @param[in] param the parameter to look for
 * @param[out] out_value the address where to store the value string
 * @param[in] max_out_len the maximum allowed output length of the parameter string
 * @pre[tested] filename must not be null
 * @pre[tested] filename must open(read) correctly
 * @pre[tested] param length must be <= 254 bytes
 * @return 0 on success, other on failure
 */
int APPCFG_GetConfigParamFromFile(
	const char* const filename,
	const char* const param,
	char* const out_value,
	const size_t max_out_len);

/**
 * @brief Parse config file for getting the configuration numeric parameter
 * @param[in] filename the filename where to find the param and its value
 * @param[in] param the parameter to look for
 * @param[out] out_value the address where to store the value string
 * @param[in] value_length the output size of the parameter type (in bytes)
 * @pre[untested] filename must not be null
 * @pre[untested] filename must open (read) correctly
 * @pre[untested] param length must be <= 254 bytes
 * @pre[untested] out_value must be not null
 * @return 0 on success, other on failure
 */
int APPCFG_GetConfigNumericParamFromFile(
	const char* const filename,
	const char* const param,
	void* const out_value,
	const size_t value_length);

/* ----------------------------------------------------------------------
 * exported variables declaration section
 * ----------------------------------------------------------------------*/

#endif /* if !defined(CONFIG_H_INCLUDE)*/

