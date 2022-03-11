#ifndef DOMAIN_H
#define DOMAIN_H

// ChineseTester_D_NewVegasEdition_V0.7
// ChineseTester_R_NewVegasEdition_V0.7

//------------------------------------------------------------

#if 0
#define LOG_TO_FILE_OUTPUT
#endif

#if 0
#define PIPE_LOGS
#endif

#if 1
#define COMPILER_LOG_OUTPUT
#endif

#if 1
#define CODE_OUTPUT_LOG
#endif

#if 0
#define TRUE_ONLY_WAY
#endif

//------------------------------------------------------------

#if defined(WIN32) || defined(WIN_BY_ME)
#define BILL_WINDOWS
#endif

#ifndef BILL_WINDOWS
#define LINUS_LINUX
#endif

//------------------------------------------------------------

#ifdef _DEBUG
#define _DBG_
#define CHINESE_DEBUG
#endif

#ifdef CHINESE_DEBUG

#define _DBG_
#define GOOD_DEBUG_ERRORS
#endif

//------------------------------------------------------------

typedef signed char		    sint_8;
typedef unsigned char	    uint_8;

typedef signed short	    sint_16;
typedef unsigned short	    uint_16;

typedef signed long		    sint_32;
typedef unsigned long	    uint_32;

typedef signed long	long	sint_64;
typedef unsigned long long	uint_64;

//------------------------------------------------------------

#include <cstring>

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>

#ifdef BILL_WINDOWS
#include <windows.h>
#endif

#endif // !DOMAIN_H