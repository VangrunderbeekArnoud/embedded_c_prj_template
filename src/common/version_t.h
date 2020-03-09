#if !defined (VERSION_T_H_INCLUDE)
#define VERSION_T_H_INCLUDE
/**
 * @file version_t.h
 * @brief interface type declarations for the IG version information
 * @code{.ebp}
 * format: 1TBS
 * rules:
 * @endcode
 */

#define xstr(s) str(s)
#define str(s) #s

#ifndef MAJ_VER
#define MAJ_VER 0
#endif
#ifndef MIN_VER
#define MIN_VER 0
#endif
#ifndef DEV_VER
#define DEV_VER 0
#endif
#ifndef GIT_HASH
#define GIT_HASH unknown
#endif
#ifndef GIT_BRANCH
#define GIT_BRANCH unknown
#endif
#ifndef BUILD_NUMBER
#define BUILD_NUMBER 0
#endif
#ifndef BUILD_DATE
#define BUILD_DATE unknown
#endif


/* ----------------------------------------------------------------------
 * include section
 * ----------------------------------------------------------------------*/
/* general includes (unix a.o.) - if possible alphabetically ordered */

/* project specific includes (Diaser IG) - if possible alphabetically ordered */

/* module specific includes (skeleton) - if possible alphabetically ordered */

/* ----------------------------------------------------------------------
 * macro declaration section
 * ----------------------------------------------------------------------*/

const volatile static int maj_ver = MAJ_VER;
const volatile static int min_ver = MIN_VER;
const volatile static int dev_ver = DEV_VER;
const static char* git_hash = xstr(GIT_HASH);
const static char* git_branch = xstr(GIT_BRANCH);
const volatile static int build_number = BUILD_NUMBER;
const static char* build_date = xstr(BUILD_DATE);


/* ----------------------------------------------------------------------
 * type declaration section
 * ----------------------------------------------------------------------*/

#endif /* if !defined(VERSION_T_H_INCLUDE) */

