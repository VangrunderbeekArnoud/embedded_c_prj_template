#if !defined (VERSION_H_INCLUDE)
#define VERSION_H_INCLUDE
/**
 * @file version.h
 * @brief functional interface declarations for IG version information
 * @code{.ebp}
 * format: 1TBS
 * rules:
 * @endcode
 */

/* ----------------------------------------------------------------------
 * include section
 * ----------------------------------------------------------------------*/
/* general includes (unix a.o.) - if possible alphabetically ordered */

/* project specific includes (Diaser IG) - if possible alphabetically ordered */

/* module specific includes (skeleton) - if possible alphabetically ordered */

/* component include */

/* ----------------------------------------------------------------------
 * function declaration section
 * ----------------------------------------------------------------------*/

/**
 * @brief extensive print of the app version
 */
void APPVER_PrintVersion();
/**
 * @brief brief print of the app version
 */
void APPVER_PrintSoftwareVersion();
/**
 * @brief prints the svn version number
 */
void APPVER_PrintGitHash();
/**
 * @brief prints the current svn branch
 */
void APPVER_PrintGitBranch();
/**
 * @brief prints the current build number
 */
void APPVER_PrintBuildNumber();
/**
 * @brief prints the current build date
 */
void APPVER_PrintBuildDate();
/**
 * @brief compile version number
 * @return char* to the brief app version
 */
char* APPVER_GetSoftwareVersion();

/* ----------------------------------------------------------------------
 * exported variables declaration section
 * ----------------------------------------------------------------------*/

#endif /* if !defined(VERSION_H_INCLUDE)*/

