#if !defined (LOG_H_INCLUDE)
#define LOG_H_INCLUDE
/**
 * @file log.h
 * @brief functional interface declarations for the common logging
 * @code{.ebp}
 * format: 1TBS
 * rules:
 * @endcode
 */

/* ----------------------------------------------------------------------
 * include section
 * ----------------------------------------------------------------------*/
/* general includes (unix a.o.) - if possible alphabetically ordered */
#include <stdbool.h>

/* project specific includes - if possible alphabetically ordered */

/* module specific includes (log) - if possible alphabetically ordered */

/* component include */
#include "log_t.h"

/* ----------------------------------------------------------------------
 * function declaration section
 * ----------------------------------------------------------------------*/

/**
 * @brief Initialize the log component.
 * @return true if init was successful, false otherwise.
 */
bool APPLOG_Init(void);

/**
 * @brief Destroy the log component.
 * @return true if the breakdown was successful, false otherwise.
 */
bool APPLOG_Breakdown(void);

/**
 * @brief Set log flags
 * @param[in] bits the set of bits to activate
 */
void APPLOG_SetLogBits(const uint64_t bits);

/**
 * @brief Reset log flags
 * @param[in] bits the set of bits to in-activate
 */
void APPLOG_ResetLogBits(const uint64_t bits);

/**
 * @brief Set the combination of log level bits
 * as defined by LOGLV_xxx defines
 * @param[in] level_bits
 * @pre[tested] level_bits must use bit values > LOGLV_UNDEFINED
 * and < LOGLV_SENTINEL
 */
void APPLOG_SetLogLevel(const uint64_t level_bits);

/**
 * @brief Print to console conditionally with time reference and
 * log level indications
 * @param[in] fn the function name
 * @param[in] bits the debug bit(s) to use
 * @param[in] fmt, ... the variable arguments list (printf-like)
 * @pre[untested] fn must point to a valid string
 * @pre[untested] fmt, ... must be properly set
 * @pre[tested] bits must only use values > LOGBIT_UNDEFINED and
 * < LOGBIT_SENTINEL
 * @pre[tested] level_bits must use bit values > LOGLV_UNDEFINED
 * and < LOGLV_SENTINEL
 */
void APPLOG_LogDebug(
		const char* fn,
		const uint64_t bits,
		char* fmt, ...);

/**
 * @brief Print to console with time reference and log level indications
 * @param[in] fn the function name
 * @param[in] level the log level to use - refer to APPLOG_LEVELS_E
 * @param[in] fmt, ... the variable arguments list (printf-like)
 * @pre[untested] fn must point to a valid string
 * @pre[untested] fmt, ... must be properly set
 * @details
 * Issue a console print if log level is set or is unknown/illegal.
 */
void APPLOG_Log(
		const char* fn,
		const uint64_t level,
		char* fmt, ...);

/* ----------------------------------------------------------------------
 * exported variables declaration section
 * ----------------------------------------------------------------------*/

#endif /* if !defined(LOG_H_INCLUDE)*/

