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
#include <stdlib.h>
#include <string.h>

/* project specific includes - if possible alphabetically ordered */
#include "../common/log.h"
#include "../common/argparse.h"
#include "../common/config.h"

/* module specific includes - if possible alphabetically ordered */
#include "../common/version.h"
#include "../common/timers.h"

timer_t p_timer_id;

/**
 * @brief Signal handler that terminates the application.
 * @param[in] sig_num the signal that raises the application termination.
 */
static void IGAPP_Term( int sig_num);
/**
 * @brief Stops the IGAPP when not null.
 */
static volatile sig_atomic_t IGAPP_done = 0; 

static const char* const usages[] = {
	"main [options] [[--] args]",
	"main [options]",
	NULL
};

/**
 * @brief Argparser for the main function
 * @param[in] argc the number of command-line arguments
 * @param[in] argv the pointer to the arguments string collection
 */
void parser(int argc, const char** argv) 
{
	int debug = 0;
	struct argparse_option options[] = {
		OPT_HELP(),
		OPT_GROUP( "Required arguments"),
		OPT_GROUP( "Optional arguments"), 
		OPT_BOOLEAN( 'd', "debug", &debug, "Set the debug flag", NULL, 0, 0),
		OPT_BOOLEAN( 'v', "version", NULL, "Display the software version", (void*)APPVER_PrintVersion, 0, 0),
		OPT_END(),
	};
	struct argparse argparse;
	argparse_init(&argparse, options, usages, 0);
	argparse_describe( &argparse, "\nBrief description of what the program does and how it works.",\
	"\nExtensive description of what the program does and how it works.");
	argc = argparse_parse(&argparse, argc, argv);
	if ( debug == 1)
		APPLOG_SetLogBits(LOGBIT_DEBUG);
}

void ReadConfig()
{
	const char* filename = "config.cfg";
	const char* string_param = "STRING_PARAM";
	const char* numeric_param = "NUMERIC_PARAM";

	char out_str_value[200];
	int out_num_value;

	if (APPCFG_GetConfigParamFromFile(filename, string_param, out_str_value, sizeof(out_str_value))) {
		APPLOG_Log(__FUNCTION__, LOGLV_ERROR, "Failed retreiving %s from %s", string_param, filename);
	}
	else {
		APPLOG_Log(__FUNCTION__, LOGLV_INFO, "Successfully read config param %s from %s: %s", string_param, filename, out_str_value);
	}
	if (APPCFG_GetConfigNumericParamFromFile(filename, numeric_param, &out_num_value, sizeof(out_num_value))) {
		APPLOG_Log(__FUNCTION__, LOGLV_ERROR, "Failed retreiving %s from %s", numeric_param, filename);
	}
	else {
		APPLOG_Log(__FUNCTION__, LOGLV_INFO, "Successfully read config param %s from %s: %d", numeric_param, filename, out_num_value);
	}
}

void timer_callback(int sig, siginfo_t *si, void *uc)
{
	int number = *(int*) si->si_value.sival_ptr;
	APPLOG_Log(__FUNCTION__, LOGLV_INFO, "Timer went off. Set new timer counting from: %d", number);
	TIMER_SetTime(&p_timer_id, number);
}

/**
 * @brief The main function
 * @param[in] argc the number of command-line arguments
 * @param[in] argv the pointer to the arguments string collection
 * @return Return value from the main
 * @retval 0 if success, other values if failure
 * @details put the details here
 * @todo put todo here
 */
int main(int argc, const char **argv)
{
	static char* fn = "main";
	int rv = -1;

	parser(argc, argv);

	signal(SIGINT, IGAPP_Term);

	if ( !APPLOG_Init()) {
		printf("%s: Couldn't initialize log component => Quit.", fn);
		rv = -2;
	} else {
		char* version = APPVER_GetSoftwareVersion();
		APPLOG_Log( fn, LOGLV_INFO, "Start APP with pid %d and software version %s", getpid(), version);
		free(version);
	}

	// Timer create
	p_timer_id = malloc(sizeof(timer_t));
	int p_param = 2;
	TIMER_CreateTimer(&p_timer_id, 5, &p_param, &timer_callback);

	//starting threads here ...
	APPLOG_LogDebug( fn, LOGBIT_DEBUG, "debugmessage");
	APPLOG_Log( fn, LOGLV_INFO, "The program has started. Use CTRL-C for stopping.");

	ReadConfig();

	

	while( 0 == IGAPP_done) {
		sleep(1);
	}

	APPLOG_Log( fn, LOGLV_INFO, "Interruption signal received. Stop APP.");
	// project breakdown ...
	APPLOG_Breakdown();
	rv = 0;
	sleep(1);
	return rv;
}

static void IGAPP_Term( int sig_num)
{
	IGAPP_done = 1;
}
