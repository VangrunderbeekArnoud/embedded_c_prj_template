/**
 * @file main.c
 * @brief logging application
 */

/* -------------------------------------------------------------------------
 * include section
 * -------------------------------------------------------------------------*/

/* general includes (unix a.o.) - if possible alphabetically ordered */
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/* project specific includes - if possible alphabetically ordered */
#include "../common/log.h"

/* module specific includes - if possible alphabetically ordered */
#include "../common/version.h"

/**
 * @brief Signal handler that terminates the application.
 * @param[in] sig_num the signal that raises the application termination.
 */
static void IGAPP_Term( int sig_num);
/**
 * @brief Stops the IGAPP when not null.
 */
static volatile sig_atomic_t IGAPP_done = 0; 

/**
 * @brief The main function
 * @param[in] argc the number of command-line arguments
 * @param[in] argv the pointer to the arguments string collection
 * @return Return value from the main
 * @retval 0 if success, other values if failure
 * @details put the details here
 * @todo put todo here
 */
int main(int argc, char *argv[])
{
	static char* fn = "main";
	int rv = -1;

	signal(SIGINT, IGAPP_Term);

	if ( !APPLOG_Init()) {
		printf("%s: Couldn't initialize log component => Quit.", fn);
		rv = -2;
	} else {
		APPLOG_Log( fn, LOGLV_INFO, "Start APP with pid %d", getpid());
		APPVER_PrintVersion();
	}

	//starting threads here ...

	APPLOG_Log( fn, LOGLV_INFO, "The program has started. Use CTRL-C for stopping.");
	while( 0 == IGAPP_done) {
		sleep(1);
	}

	APPLOG_Log( fn, LOGLV_INFO, "Interruption signal received. Stop APP.");
	// project breakdown ...
	rv = 0;
	sleep(1);
	return rv;
}

static void IGAPP_Term( int sig_num)
{
	IGAPP_done = 1;
}
