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
#include <time.h>

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
#define TIMER_MIN SIGRTMIN		/**< The minimum signal number to use */
#define TIMER_MAX 30 			/**< Max number of timers that can be created */

static int TIMER_Counter = 0; 	/**< The total number of timers created */

/**
 * @brief struct array to store key <timer_id> value <signum> pairs
 */
struct TIMER_s {
	timer_t timer_id;
	int signum;
};
struct TIMER_s TIMER_Instances[TIMER_MAX];
/**
 * @brief return the index of the array for a timer id.
 * @return 0 on success, else on failure
 */
static int TIMER_GetIndex(const timer_t* p_timer_id, int* p_index);
/**
 * @brief Get the value pair for key timer_id
 * @return 0 on failure, else on success
 */
static int TIMER_GetSigNum(timer_t* p_timer_id);
/**
 * @brief function to determine if a timer already exists
 * @return true when it exists, false if not
 */
static bool TIMER_Exists(timer_t* p_timer_id);
/**
 * @brief Gets the first free signum
 * @return 0 on success, other on failure
 */
static int TIMER_GetFreeSignum(int* signum);
/**
 * @brief push a timer id and signum to the timer array
 * @return 0 on success, other on failure
 */
static int TIMER_PushTimer(timer_t* p_timer_id, int signum);
/**
 * @brief pops a timer id and signum from the timer array
 * @return 0 on success, other on failure
 */
static int TIMER_PopTimer(const timer_t* p_timer_id);

static int TIMER_GetIndex(const timer_t* p_timer_id, int* p_index)
{
	int rv = -1;
	for (int i = 0; i < TIMER_MAX; i++) {
		if (*p_timer_id == TIMER_Instances[i].timer_id) {
			*p_index = i;
			rv = 0;
		}
	}
	return rv;
}
static int TIMER_GetSigNum(timer_t* p_timer_id) 
{
	int index, rv;
	if (TIMER_GetIndex(p_timer_id, &index)) {
		rv = 0;
	} else {
		rv = TIMER_Instances[index].signum;
	}
	return rv;
}
static bool TIMER_Exists(timer_t* p_timer_id)
{
	if (TIMER_GetSigNum(p_timer_id)) {
		return true;
	} else {
		return false;
	}
}
static int TIMER_GetFreeSignum(int* signum)
{
	int rv = -1;
	for (int i = 0; i <= TIMER_MAX; i++) {
		if (TIMER_Instances[i].signum == 0) { 
			*signum = TIMER_MIN+i;
			rv = 0;
			break;
		}
	}
	return rv;
}
static int TIMER_PushTimer(timer_t* p_timer_id, int signum)
{
	int rv = -1;
	TIMER_Instances[signum-TIMER_MIN].timer_id = *p_timer_id;
	TIMER_Instances[signum-TIMER_MIN].signum = signum;
	TIMER_Counter++;
	rv = 0;
	return rv;

}
static int TIMER_PopTimer(const timer_t* p_timer_id)
{
	int index, rv;
	if (TIMER_GetIndex(p_timer_id, &index)) {
		APPLOG_Log(__FUNCTION__, LOGLV_WARNING, "timer %d does not exist", *p_timer_id);
		rv = -1;
	} else {
		TIMER_Instances[index].timer_id = NULL;
		TIMER_Instances[index].signum = 0;
		TIMER_Counter--;
		rv = 0;
	}
	return rv;
}
int TIMER_CreateTimer(
		timer_t* const p_timer_id,
		const uint32_t seconds,
		const void* const p_params,
		TIMERS_CallBack_FP call_back)
{
	static const char* fn = "TIMER_CreateTimer";
	int rv = -1, signum;
	struct sigevent  sig_event;
	struct sigaction sig_action;
	sigset_t sig_mask;

	if (NULL == p_timer_id){
		APPLOG_Log(fn, LOGLV_CRITICAL, "Null pointer to timer id");
		rv = -1;
	} else if (NULL == call_back){
		APPLOG_Log(fn, LOGLV_CRITICAL, "Null call-back pointer");
		rv = -1;
	} else if (TIMER_Exists(p_timer_id)) {
		APPLOG_Log(fn, LOGLV_WARNING, "timer_id %d already in use", *p_timer_id);
		rv = -1;
	} else if (TIMER_Counter > TIMER_MAX) {
		APPLOG_Log(fn, LOGLV_CRITICAL, "Cannot create timer %d, all signals in use", *p_timer_id);
		rv = -1;
	} else if (TIMER_GetFreeSignum(&signum)) {
		APPLOG_Log(fn, LOGLV_WARNING, "Unable to find free Signum");
		rv = -1;
	} else {
		sig_action.sa_flags = SA_SIGINFO;
		sig_action.sa_sigaction = call_back;
		sigemptyset(&sig_action.sa_mask);

		if (0 > sigaction(signum, &sig_action, NULL)){
			APPLOG_Log(fn, LOGLV_ERROR, "Couldn't create timer: %s", strerror(errno));
		} else {
			/* Block timer signal temporarily */
			sigemptyset(&sig_mask);
			sigaddset(&sig_mask, signum);

			if (0 > sigprocmask(SIG_SETMASK, &sig_mask, NULL)){
				APPLOG_Log(fn, LOGLV_ERROR, "Couldn't block timer: %s", strerror(errno));
			} else {
				/* Create timer */
				sig_event.sigev_notify          = SIGEV_SIGNAL;
				sig_event.sigev_signo           = signum;
				sig_event.sigev_value.sival_ptr = (void*) p_params;

				if (0 > timer_create(TIMER_CLOCKID, &sig_event, p_timer_id)){
					APPLOG_Log(fn, LOGLV_ERROR, "Couldn't create timer: %s", strerror(errno));
				} else if (0 != TIMER_SetTime(*p_timer_id, seconds)){
					APPLOG_Log(fn, LOGLV_ERROR, "Couldn't set time: %s", strerror(errno));
				} else if (0 > sigprocmask(SIG_UNBLOCK, &sig_mask, NULL)){
					APPLOG_Log(fn, LOGLV_ERROR, "Couldn't unblock signal: %s", strerror(errno));
				} else {
					TIMER_PushTimer(p_timer_id, signum);
					APPLOG_Log(fn, LOGLV_INFO, "timer %d successfully created with signum %d", *p_timer_id, signum);
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
		TIMER_DisposeTimer(timer_id);
	} else {
		rv = 0;
	}
	return rv;
}

/* ----------------------------------------------------------------------*/
int TIMER_DisposeTimer(const timer_t timer_id)
{
	static const char* fn = "TIMER_DisposeTimer";
	int rv = -1;

	if (0 > timer_delete(timer_id)){
		APPLOG_Log(fn, LOGLV_WARNING, "Couldn't delete timer: %s", strerror(errno));
	} else {
		TIMER_PopTimer(&timer_id);
		APPLOG_Log(fn, LOGLV_INFO, "timer %d successfully disposed", timer_id);
		rv = 0;
	}
	return rv;
}

/* ----------------------------------------------------------------------
 * functions with internal linkage (static) definition section
 * ----------------------------------------------------------------------*/

