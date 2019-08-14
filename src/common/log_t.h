#if !defined (LOG_T_H_INCLUDE)
#define LOG_T_H_INCLUDE
/**
 * @file log_t.h
 * @brief interface type declarations for common logging
 * @code{.ebp}
 * format: 1TBS
 * rules:
 * @endcode
 */

/* ----------------------------------------------------------------------
 * include section
 * ----------------------------------------------------------------------*/
/* general includes (unix a.o.) - if possible alphabetically ordered */
#include <stdint.h>

/* project specific includes - if possible alphabetically ordered */

/* module specific includes (log) - if possible alphabetically ordered */

/* ----------------------------------------------------------------------
 * macro declaration section
 * ----------------------------------------------------------------------*/

/* log level bits values */
#define LOGLV_UNDEFINED (0x0000L) /**< DO NOT USE */
#define LOGLV_INFO      (0x0001L)
#define LOGLV_DEBUG     (0x0002L)
#define LOGLV_WARNING   (0x0004L)
#define LOGLV_ERROR     (0x0008L)
#define LOGLV_CRITICAL  (0x0010L)
#define LOGLV_TEST      (0x0020L)
#define LOGLV_SENTINEL  (0x0040L) /**< DO NOT USE */

/* log debug bits values - only used in DEBUG log level */
#define LOGBIT_UNDEFINED (0x0000L) /**< DO NOT USE */
#define LOGBIT_TCPIPSRV  (0x0001L) /**< TCP-IP Server details */
#define LOGBIT_APPTCPIP   (0x0002L) /**< APP TCP-IP details */
#define LOGBIT_DIAFSM    (0x0004L) /**< APP Diaser Messaging FSM details */
#define LOGBIT_DIAMSGS   (0x0008L) /**< Diaser messages details */
#define LOGBIT_STVAL     (0x0010L) /**< Structure validation details */
#define LOGBIT_CONNDICT  (0x0020L) /**< Diaser connections dictionary */
#define LOGBIT_FSMEN     (0x0040L) /**< FSM Engine */
#define LOGBIT_MEMALLOC  (0x0080L) /**< Memory allocation */
#define LOGBIT_CMDBR     (0x0100L) /**< Command Broker */
#define LOGBIT_OCCLI     (0x0200L) /**< OCIT-C Client */
#define LOGBIT_OCSRV     (0x0400L) /**< OCIT-C Server */
#define LOGBIT_SENTINEL  (0x0800L) /**< DO NOT USE */

/**
 * @brief Print to console with time reference and log level indications
 * if conditions are fulfilled
 * @param[in] cond the condition to fulfill
 * @param[in] fn the function name
 * @param[in] level the log level to use - refer to APPLOG_LEVELS_E
 * @param[in] fmt, ... the variable arguments list (printf-like)
 * @pre[untested] fn must point to a valid string
 * @pre[untested] fmt, ... must be properly set
 * @details
 * Issue a console print if log level is set or is unknown/illegal.
 */
#define APPLOG_CondLog(cond, fn, level, fmt, ...) { \
	if (cond) {                                    \
		APPLOG_Log(fn, level, fmt, __VA_ARGS__);    \
	}                                              \
}

/* ----------------------------------------------------------------------
 * type declaration section
 * ----------------------------------------------------------------------*/

#endif /* if !defined(LOG_T_H_INCLUDE) */

