/**
 * @file version.c
 * @brief implementation of the version information
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
#include "log.h"

/* module specific includes - if possible alphabetically ordered */

/* component include */
#include "version.h"

/* ----------------------------------------------------------------------
 * internal macro declaration section
 * ----------------------------------------------------------------------*/

/* ----------------------------------------------------------------------
 * internal type declaration section
 * ----------------------------------------------------------------------*/

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
void APPVER_PrintVersion(void)
{
	APPLOG_Log("APPVER_PrintVersion", LOGLV_INFO, "APP software version %02d.%02d (%02d)", VER_MAJ, VER_MIN, VER_DEV);
}


/* ----------------------------------------------------------------------
 * functions with internal linkage (static) definition section
 * ----------------------------------------------------------------------*/

