/**
 * @file log.c
 * @brief implementation of the common logging
 * @code{.ebp}
 * format: 1TBS
 * rules:
 * @endcode
 */

/* ----------------------------------------------------------------------
 * include section
 * ----------------------------------------------------------------------*/
/* general includes (unix a.o.) - if possible alphabetically ordered */
#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

/* project specific includes - if possible alphabetically ordered */

/* module specific includes (log) - if possible alphabetically ordered */

/* component include */
#include "log.h"

/* ----------------------------------------------------------------------
 * internal macro declaration section
 * ----------------------------------------------------------------------*/

/** The default log level bits combination */
#define APPLOG_DEFAULT_LEVEL_BITS (LOGLV_INFO |  LOGLV_DEBUG | LOGLV_WARNING | LOGLV_ERROR | LOGLV_CRITICAL | LOGLV_TEST)

/** The default debug bits combination */
#define APPLOG_DEFAULT_DEBUG_BITS (	LOGBIT_UNDEFINED);

#define ANSI_COLOR_RED     "\x1b[91m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_YELLOW  "\x1b[93m"
#define ANSI_COLOR_BLUE    "\x1b[94m"
#define ANSI_COLOR_MAGENTA "\x1b[95m"
#define ANSI_COLOR_CYAN    "\x1b[96m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/**
 * @brief Semaphore value
 */
#define APPLOG_RESOURCE_IDLE (1)

/**
 * @brief Semaphore value
 */
#define APPLOG_RESOURCE_BUSY (0)

/* ----------------------------------------------------------------------
 * internal type declaration section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * function with internal linkage (static) declaration section
 * ----------------------------------------------------------------------*/

/**
 * @brief Print time reference, function name and log level to console
 * @param[in] fn the function name
 * @param[in] level_str the log level string
 * @pre[untested] fn must point to a valid function string
 * @pre[untested] level_str must point to a valid log level string
 */
static void APPLOG_TimeNCo(const char* fn, const char* level_str);

/**
 * @brief Get access to log resource
 * @return true if access is granted, false otherwise
 */
static bool APPLOG_GetLogAccess(void);

/**
 * @brief Release access to log resource
 * @return true if access is released, false otherwise
 */
static bool APPLOG_ReleaseLogAccess(void);

/* ----------------------------------------------------------------------
 * variable with internal linkage (static) definition section
 * ----------------------------------------------------------------------*/

static uint64_t APPLOG_level_bits = APPLOG_DEFAULT_LEVEL_BITS; /**< The current log level bits combination */
static uint64_t APPLOG_debug_bits = APPLOG_DEFAULT_DEBUG_BITS; /**< The current debug bits combination */

/**
 * @brief The address of the log resource access semaphore
 */
static sem_t APPLOG_log_busy_mutex;

/**
 * @brief State variable representing the initialized state of the component
 */
static bool APPLOG_is_init;

/* ----------------------------------------------------------------------
 * exported variable definition section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * exported function definition section
 * ----------------------------------------------------------------------*/

/* --------------------------------------------------------------------- */
bool APPLOG_Init(void)
{
	static const char* fn="APPLOG_Init";
	bool rv = false;
	int init_res;

	if (APPLOG_is_init){
		printf("%s: [ERROR] Log component already initialized!", fn);
	} else{
		init_res = sem_init(&APPLOG_log_busy_mutex, 0, APPLOG_RESOURCE_IDLE);

		if(0 > init_res){
			printf("%s: [ERROR] Couldn't initialize semaphore:%s", fn, strerror(errno));
		} else {
			APPLOG_is_init = true;
			rv = true;
		}
	}

	return rv;
}

/* --------------------------------------------------------------------- */
bool APPLOG_Breakdown(void)
{
	static const char* fn="APPLOG_Breakdown";
	bool rv = false;
	int get_value_result;
	int destroy_result;
	int mutex_val;

	if (!APPLOG_is_init){
		printf("%s: [ERROR] Log component not initialized!", fn);
	} else{

		get_value_result = sem_getvalue(&APPLOG_log_busy_mutex, &mutex_val);

		if (0 > get_value_result){
			printf("%s: [ERROR] Couldn't get semaphore value:%s", fn, strerror(errno));
		} else if (APPLOG_RESOURCE_IDLE != mutex_val){
			APPLOG_Log(fn, LOGLV_WARNING, "Semaphore value unexpected:%d", mutex_val);
		}

		destroy_result = sem_destroy(&APPLOG_log_busy_mutex);

		if (0 > destroy_result){
			APPLOG_Log(fn, LOGLV_WARNING, "Couldn't destroy semaphore:%s", strerror(errno));
		} else {
			rv = true;
		}

		APPLOG_is_init = false;
	}
	return rv;
}

/* ----------------------------------------------------------------------*/
void APPLOG_SetLogBits(const uint64_t bits)
{
	APPLOG_debug_bits |= bits;
}

/* ----------------------------------------------------------------------*/
void APPLOG_ResetLogBits(const uint64_t bits)
{
	APPLOG_debug_bits &= ~bits;
}

/* ----------------------------------------------------------------------*/
void APPLOG_LogDebug(
		const char* fn,
		const uint64_t bits,
		char* fmt, ...)
{
	va_list args;

	if (APPLOG_level_bits & LOGLV_DEBUG){
		if (bits & APPLOG_debug_bits){
			if (APPLOG_GetLogAccess()){
				APPLOG_TimeNCo(fn, "DEBUG");

				va_start(args, fmt);
				vprintf(fmt, args);
				va_end(args);
				putc('\n', stdout);

				if (!APPLOG_ReleaseLogAccess()){
					printf("APPLOG_LogDebug: Releasing log access fails!");
				}
			}
		}
	}
}

/* ----------------------------------------------------------------------*/
void APPLOG_Log(
		const char* fn,
		const uint64_t level,
		char* fmt, ...)
{
	char level_str[20] = "";
	bool log_out = false;
	va_list args;

	if (LOGLV_UNDEFINED == level || LOGLV_SENTINEL < level){
		strcat(level_str, "????");
		log_out = true;
	} else {
		if      (level & LOGLV_INFO)     strcat(level_str, ANSI_COLOR_GREEN   "INFO"     ANSI_COLOR_RESET);
		else if (level & LOGLV_DEBUG)    strcat(level_str, ANSI_COLOR_BLUE    "DEBUG"    ANSI_COLOR_RESET);
		else if (level & LOGLV_WARNING)  strcat(level_str, ANSI_COLOR_YELLOW  "WARNING"  ANSI_COLOR_RESET);
		else if (level & LOGLV_ERROR)    strcat(level_str, ANSI_COLOR_RED     "ERROR"    ANSI_COLOR_RESET);
		else if (level & LOGLV_CRITICAL) strcat(level_str, ANSI_COLOR_RED     "CRITICAL" ANSI_COLOR_RESET);
		else if (level & LOGLV_TEST)     strcat(level_str, ANSI_COLOR_MAGENTA "TEST"     ANSI_COLOR_RESET);
	}

	if (level & APPLOG_level_bits){
		log_out = true;
	}

	if(log_out){
		if (APPLOG_GetLogAccess()){
			APPLOG_TimeNCo(fn, level_str);

			va_start(args, fmt);
			vprintf(fmt, args);
			va_end(args);
			putc('\n', stdout);

			if (!APPLOG_ReleaseLogAccess()){
				printf("APPLOG_LogDebug: Releasing log access fails!");
			}
		}
	}
}

/* ----------------------------------------------------------------------
 * functions with internal linkage (static) definition section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------*/
static void APPLOG_TimeNCo(const char* fn, const char* level_str)
{
	struct timeval current_time_fine;
	time_t current_time;
	struct tm broken_time;

	time(&current_time);
	gettimeofday(&current_time_fine, NULL);
	{
		char time_str[20];
		unsigned long ms = current_time_fine.tv_usec/1000;

		strftime(time_str, 20, "%Y-%m-%d %X", localtime_r(&current_time, &broken_time));
		printf("%s.%lu) [%s] %s: ", time_str, ms, level_str, fn);
	}
}

/* --------------------------------------------------------------------- */
static bool APPLOG_GetLogAccess(void)
{
	static const char* fn = "APPLOG_GetLogAccess";
	bool rv = false;
	int wait_res;

	if (!APPLOG_is_init){
		printf("%s: Log component not initialized!\n", fn);
	} else {
		/** @todo implement timeout */
		wait_res = sem_wait(&APPLOG_log_busy_mutex);

		if (0 > wait_res){
			APPLOG_Log(fn, LOGLV_ERROR, "Error getting memory resource access:%s", strerror(errno));
		} else if (APPLOG_RESOURCE_BUSY != wait_res){
			APPLOG_Log(fn, LOGLV_ERROR, "Illegal semaphore value:%d", wait_res);
		} else {
			rv = true;
		}
	}
	return rv;
}

/* --------------------------------------------------------------------- */
static bool APPLOG_ReleaseLogAccess(void)
{
	static const char* fn = "APPLOG_ReleaseLogAccess";
	bool rv = false;
	int post_res;
	int val_res;
	int val;

	if (!APPLOG_is_init){
		printf("%s: Log component not initialized!\n", fn);
	} else {
		/** @todo implement timeout */
		post_res = sem_post(&APPLOG_log_busy_mutex);

		if (0 > post_res){
			APPLOG_Log(fn, LOGLV_ERROR, "Coudn't release memory resource access:%s", strerror(errno));
		} else {
			val_res = sem_getvalue(&APPLOG_log_busy_mutex, &val);

			if (0 > val_res){
				APPLOG_Log(fn, LOGLV_ERROR, "Coudn't get memory resource access semaphore value:%s", strerror(errno));
			} else if ((APPLOG_RESOURCE_IDLE != val) && (APPLOG_RESOURCE_BUSY != val)){
				APPLOG_Log(fn, LOGLV_ERROR, "Illegal memory resource access semaphore value:%d", val);
			} else {
				rv = true;
			}
		}
	}
	return rv;
}
