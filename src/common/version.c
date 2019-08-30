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
#include <stdio.h>
#include <stdlib.h>

/* project specific includes - if possible alphabetically ordered */

/* module specific includes - if possible alphabetically ordered */

/* component include */
#include "version.h"
#include "version_t.h"

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

void APPVER_PrintVersion()
{
  printf("\n");
  APPVER_PrintSoftwareVersion();
  printf("\n");
  APPVER_PrintSvnVersion();
  APPVER_PrintSvnBranch();
  printf("\n");
  APPVER_PrintBuildNumber();
  APPVER_PrintBuildDate();
  printf("\n");
  exit(1);
}

void APPVER_PrintSoftwareVersion(void)
{
  printf("APP software version %02d.%02d (%02d)\n", maj_ver, min_ver, dev_ver);
}

char* APPVER_GetSoftwareVersion(void)
{
  char* version = malloc(sizeof(char) * 20);
  snprintf(version, 20, "%02d.%02d (%02d)", maj_ver, min_ver, dev_ver);
  return version;
}

void APPVER_PrintSvnVersion()
{
  printf("SVN version \t%s\n", svn_ver);
}

void APPVER_PrintSvnBranch()
{
  printf("SVN branch \t%s\n", svn_branch);
}

void APPVER_PrintBuildNumber()
{
  printf("Build number \t%s\n", build_number);
}

void APPVER_PrintBuildDate()
{
  printf("Build date \t%s\n", build_date);
}

/* ----------------------------------------------------------------------
 * functions with internal linkage (static) definition section
 * ----------------------------------------------------------------------*/

