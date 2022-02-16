#ifndef DOMAIN_H
#define DOMAIN_H

#define _CRT_SECURE_NO_WARNINGS

// ChineseTester_D_NewVegasEdition_V0.7
// ChineseTester_R_NewVegasEdition_V0.7

#if 0
#define LOG_TO_FILE_OUTPUT
#endif

#if 0
#define PIPE_LOGS
#endif

#if 0
#define COMPILER_LOG_OUTPUT
#endif

#if 1
#define CODE_OUTPUT_LOG
#endif

#if defined(WIN32) || defined(WIN_BY_ME)
#define BILL_WINDOWS
#endif

#ifndef BILL_WINDOWS
#define LINUS_LINUX
#endif

#ifdef _DEBUG
#define _DBG_
#define CHINESE_DEBUG
#endif

#undef  _DBG_

#ifdef CHINESE_DEBUG

#if 0
#define TRUE_ONLY_WAY
#endif

#define _DBG_
#define GOOD_DEBUG_ERRORS
#endif

#ifdef  BILL_WINDOWS
#define RESOURCES       MAIN_PATH   + "resources\\"
#define COMPILERS       RESOURCES   + "compilers\\"
#define EXAMPLES        RESOURCES   + "examples\\"
#define WORK_DIR        RESOURCES   + "working_directory\\"
#ifdef  TRUE_ONLY_WAY
#define MAEDIA          RESOURCES   + "media\\"
#else
#define MAEDIA          MAIN_PATH   + "media\\"
#endif

#define MAIN_PATH       getMainPath()
#ifdef  TRUE_ONLY_WAY
#define DB_PATH         RESOURCES   + "database\\"
#else
#define DB_PATH         MAIN_PATH   
#endif
#define DATABASE_PATH   DB_PATH     + "db.sqlite3"

#define CHECKER_PATH    WORK_DIR    + "checker\\checker"
#define SOLUTION_PATH   WORK_DIR    + "solution\\solution"
#define TEST_PATH       WORK_DIR    + "tests\\"
#define ANSWERS_PATH    WORK_DIR    + "answers\\"
#define OUTPUT_PATH     WORK_DIR    + "outputs\\"
#define RESULT_PATH     WORK_DIR    + "results\\"
#define LOGS_PATH       WORK_DIR    + "logs\\"
#elif defined(LINUS_LINUX)
#define RESOURCES       MAIN_PATH   + "resources/"
#define COMPILERS       RESOURCES   + "compilers/"
#define EXAMPLES        RESOURCES   + "examples/"
#define WORK_DIR        RESOURCES   + "working_directory/"

#define MAEDIA          MAIN_PATH   + "media/"

#define MAIN_PATH       getMainPath()
#define DB_PATH         MAIN_PATH

#define DATABASE_PATH   DB_PATH     + "db.sqlite3"

#define CHECKER_PATH    WORK_DIR    + "checker/checker"
#define SOLUTION_PATH   WORK_DIR    + "solution/solution"
#define TEST_PATH       WORK_DIR    + "tests/"
#define ANSWERS_PATH    WORK_DIR    + "answers/"
#define OUTPUT_PATH     WORK_DIR    + "outputs/"
#define RESULT_PATH     WORK_DIR    + "results/"
#define LOGS_PATH       WORK_DIR    + "logs/"
#endif

#define CPP_COMPILER_NAME   COMPILERS +   "magicCPPCompiler.cmd"

#define WINDOWS_OSS

#define _(aName) #aName

#define W_LOG(aMessege)                                 \
    {                                                   \
        std::cout <<aMessege << "\n";                   \
    }

#define W_END_LOG                                       \
    {                                                   \
        std::cout << std::endl;                         \
    }

#ifdef  GOOD_DEBUG_ERRORS
#define W_ERROR(aNumber, aMessege)                      \
    {                                                   \
        std::cout << "\n\n\x1b[31m!!!!!!!!! "           \
            "ERROR HAS OCCURRED !!!!!!!!!\n\n"          \
            << "ERROR# " #aNumber "\n"                  \
            << aMessege << "\x1b[0m\n\n" << std::endl;  \
        exit(0);                                        \
    }
#else
#define W_ERROR(aNumber, aMessege)                      \
    {                                                   \
        std::cout << std::string("\n\n!!!!!!!!! "      \
            "ERROR HAS OCCURRED !!!!!!!!!\n"            \
            "ERROR# " #aNumber "\n") +                  \
            std::string(aMessege) +                     \
            std::string( "" "\n\n" "\n\n" )             \
            << std::endl; exit(0);                      \
    }
#endif

#ifdef _DBG_
    #define WD_LOG(aMessege) W_LOG(aMessege) 
    #define WD_END_LOG W_END_LOG 
    #define WD_ERROR(aNumber, aMessege) W_ERROR(aNumber, aMessege) 
#else
    #define WD_LOG(aMessege) 
    #define WD_END_LOG
    #define WD_ERROR(aNumber, aMessege)
#endif // DEBUG

#define print(aMessage)         \
{                               \
    std::cout << aMessage;      \
}
#define _CRT_SECURE_NO_WARNINGS
typedef signed char		    sint_8;
typedef unsigned char	    uint_8;
typedef signed short	    sint_16;
typedef unsigned short	    uint_16;
typedef signed long		    sint_32;
typedef unsigned long	    uint_32;
typedef signed long	long	sint_64;
typedef unsigned long long	uint_64;

#include <cstring>

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#ifdef BILL_WINDOWS
#include <windows.h>
#endif
std::string getMainPath();
std::string getDBPath();

//std::string makeGoodString(std::string aBadString);
//std::wstring makeBadString(std::string aGoodString);
//std::string makeGoodString(std::wstring aSS);

void copyFile(std::string aFromFileName, std::string aToFileName);
//void copyFile(std::string aFromFileName, std::string aToFileName);
std::string getString(const unsigned char* aString);
std::string getString(const char* aString);
#ifdef BILL_WINDOWS
std::string makeWindowString(std::string aString);
std::string GetLastErrorAsString();
#endif
#define _CRT_SECURE_NO_WARNINGS
#define newStrCopy(str) std::strcpy(new char[str.size() + 1], str.c_str())
#define newCharPtrCopy(str) std::strcpy(new char[strlen(str) + 1], str)
#define newLargerStrCopy(str, offset) \
std::strcpy(new char[str.size() + offset + 1], str.c_str())
#define strCopy(strTo, strFrom) std::strcat(strTo, strFrom)

#endif //DOMAIN_H