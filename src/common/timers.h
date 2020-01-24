#if !defined (TIMERS_H_INCLUDE)
#define TIMERS_H_INCLUDE
/**
 * @file timers.h
 * @brief functional interface declarations for IG Timers handling
 * @code{.ebp}
 * format: 1TBS
 * rules:
 * @endcode
 */

/* ----------------------------------------------------------------------
 * include section
 * ----------------------------------------------------------------------*/
/* general includes (unix a.o.) - if possible alphabetically ordered */

/* project specific includes - if possible alphabetically ordered */

/* module specific includes (skeleton) - if possible alphabetically ordered */
#include <time.h>
/* component include */
#include "timers_t.h"

/* ----------------------------------------------------------------------
 * function declaration section
 * ----------------------------------------------------------------------*/
/**
 * @brief Initializes the timer object array
 * @return true on success, false on failure
 */
bool TIMER_Init(void);
/** 
 * @brief Destroy the TIMER component.
 * @return true if the breakdown was successful, false otherwise.
 */
bool TIMER_Breakdown(void);
/**
 * @brief Create Timer
 * @param[out] p_timer_id the address where to return the ID of the created timer
 * @param[in] seconds the interval in seconds
 * @param[in] p_fsm_params the caller FSM base params
 * @param[in] call_back the call-back function pointer that is called once the timer elapses
 * @pre[tested] p_fsm_params must not be null
 * @pre[tested] p_timer_id must not be null
 * @pre[tested] call_back must not be null
 * @return Error code that confirms that the timer creation was successful or not.
 */
int TIMER_CreateTimer(
		timer_t* const p_timer_id,
		const uint32_t seconds,
		//const struct IGFSM_FSM_PARAMS_S* const p_fsm_params,
		const void* const p_params,
		//const int* const p_fsm_params,
		TIMERS_CallBack_FP call_back);

/**
 * @brief Disarm and delete the timer
 * @param[in] timer_id the identifier of the timer to stop and delete
 * @param[in] seconds the interval (in seconds) to load the timer with
 * @return Error code that confirms that the timer loading was successful or not.
 */
int TIMER_SetTime(const timer_t timer_id, uint32_t seconds);

/**
 * @brief Disarm and delete the timer
 * @param[in, out] timer_id the identifier of the timer to stop and delete
 * @return Error code that confirms that the timer deletion was successful or not.
 */
int TIMER_DisposeTimer(const timer_t timer_id);

/* ----------------------------------------------------------------------
 * exported variables declaration section
 * ----------------------------------------------------------------------*/

#endif /* if !defined(TIMERS_H_INCLUDE)*/

