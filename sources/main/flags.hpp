#ifndef DOMAIN_HPP
#define DOMAIN_HPP

//------------------------------------------------------------
//						PROJECT SETTINGS
//------------------------------------------------------------

#if 1
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
//							OS FLAGS
//------------------------------------------------------------

#if defined(WIN32) || defined(WIN_BY_ME)
#define BILL_WINDOWS
#endif

#ifndef BILL_WINDOWS
#define LINUS_LINUX
#endif

//------------------------------------------------------------
//							DEBUG FLAGS
//------------------------------------------------------------

#ifdef _DEBUG
#define _DBG_
#define CHINESE_DEBUG
#endif

#ifdef CHINESE_DEBUG
#define GOOD_DEBUG_ERRORS
#endif


#include <cstring>
#ifdef BILL_WINDOWS
#include <windows.h>
#endif

#endif // !DOMAIN_HPP