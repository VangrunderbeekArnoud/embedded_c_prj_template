#if !defined (TIMERS_T_H_INCLUDE)
#define TIMERS_T_H_INCLUDE
/**
 * @file timers_t.h
 * @brief interface type declarations for the timers handling
 * @code{.ebp}
 * format: 1TBS
 * rules:
 * @endcode
 */

/* ----------------------------------------------------------------------
 * include section
 * ----------------------------------------------------------------------*/
/* general includes (unix a.o.) - if possible alphabetically ordered */
#include <signal.h>

/* project specific includes - if possible alphabetically ordered */

/* module specific includes (Timers) - if possible alphabetically ordered */

/* ----------------------------------------------------------------------
 * macro declaration section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * type declaration section
 * ----------------------------------------------------------------------*/

/**
 * @brief The IG Timers call-back function pointer type
 * @param[in] sig the signal that triggered the callback
 * @param[in] si the signal information structure
 * @param[in] uc the context (usually not used)
 */
typedef void (*TIMERS_CallBack_FP) (int sig, siginfo_t *si, void *uc);

#endif /* if !defined(TIMERS_T_H_INCLUDE) */

