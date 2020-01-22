/**
 * @file timers.c
 * @brief implementation of the Timers handling
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
#include <signal.h>
#include <string.h>

/* project specific includes - if possible alphabetically ordered */
#include "log.h"

/* module specific includes (App) - if possible alphabetically ordered */

/* component include */
#include "timers.h"

/* ----------------------------------------------------------------------
 * internal macro declaration section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * internal type declaration section
 * ----------------------------------------------------------------------*/

#define TIMER_CLOCKID CLOCK_REALTIME /**< The clock source used by the timers */
#define TIMER_SIG     SIGRTMIN       /**< @todo TIMER_SIG To be documented */

/* ----------------------------------------------------------------------
 * function with internal linkage (static) declaration section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * variable with internal linkage (static) definition section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * exported variable definition section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * exported function definition section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------*/
int TIMER_CreateTimer(
		timer_t* const p_timer_id,
		const uint32_t seconds,
		//const struct IGFSM_FSM_PARAMS_S* const p_fsm_params,
		const void* const p_params,
		//const int* const p_fsm_params,
		TIMERS_CallBack_FP call_back)
{
	static const char* fn = "TIMER_CreateTimer";
	int rv = -1;
	struct sigevent  sig_event;
	struct sigaction sig_action;
	sigset_t sig_mask;

	if (NULL == p_timer_id){
		APPLOG_Log(fn, LOGLV_CRITICAL, "Null pointer to timer id");
		rv = -1;
	} else if (NULL == call_back){
		APPLOG_Log(fn, LOGLV_CRITICAL, "Null call-back pointer");
		rv = -1;
	} else {
		/* Establish handler for timer signal */
		sig_action.sa_flags = SA_SIGINFO;
		sig_action.sa_sigaction = call_back;
		sigemptyset(&sig_action.sa_mask);

		if (0 > sigaction(TIMER_SIG, &sig_action, NULL)){
			APPLOG_Log(fn, LOGLV_ERROR, "Couldn't create timer: %s", strerror(errno));
		} else {
			/* Block timer signal temporarily */
			sigemptyset(&sig_mask);
			sigaddset(&sig_mask, TIMER_SIG);

			if (0 > sigprocmask(SIG_SETMASK, &sig_mask, NULL)){
				APPLOG_Log(fn, LOGLV_ERROR, "Couldn't block timer: %s", strerror(errno));
			} else {
				/* Create timer */
				sig_event.sigev_notify          = SIGEV_SIGNAL;
				sig_event.sigev_signo           = TIMER_SIG;
				sig_event.sigev_value.sival_ptr = (void*) p_params;

				if (0 > timer_create(TIMER_CLOCKID, &sig_event, p_timer_id)){
					APPLOG_Log(fn, LOGLV_ERROR, "Couldn't create timer: %s", strerror(errno));
				} else if (0 != TIMER_SetTime(*p_timer_id, seconds)){
					APPLOG_Log(fn, LOGLV_ERROR, "Couldn't set time: %s", strerror(errno));
				} else if (0 > sigprocmask(SIG_UNBLOCK, &sig_mask, NULL)){
					APPLOG_Log(fn, LOGLV_ERROR, "Couldn't unblock signal: %s", strerror(errno));
				} else {
					APPLOG_Log(fn, LOGLV_INFO, "Timer successfully created");
					rv = 0;
				}
			}
		}
	}
	return rv;
}

/* ----------------------------------------------------------------------*/
int TIMER_SetTime(const timer_t timer_id, uint32_t seconds)
{
	static const char* fn = "TIMER_SetTime";
	int rv = -1;
	struct itimerspec interval;

	interval.it_interval.tv_sec  = 0;
	interval.it_interval.tv_nsec = 0;
	interval.it_value.tv_sec     = seconds;
	interval.it_value.tv_nsec    = 0;

	if (0 > timer_settime(timer_id, 0, &interval, NULL)){
		APPLOG_Log(fn, LOGLV_CRITICAL, "Couldn't set timer interval");
		timer_delete(timer_id);
	} else {
		rv = 0;
	}
	return rv;
}

/* ----------------------------------------------------------------------*/
int TIMER_DisposeTimer(const timer_t timer_id)
{
	static const char* fn = "TIMER_DisposeDiaserTimer";
	int rv = -1;

	if (0 > timer_delete(timer_id)){
		APPLOG_Log(fn, LOGLV_WARNING, "Couldn't delete timer: %s", strerror(errno));
	} else {
		rv = 0;
	}

	return rv;
}

/* ----------------------------------------------------------------------
 * functions with internal linkage (static) definition section
 * ----------------------------------------------------------------------*/

