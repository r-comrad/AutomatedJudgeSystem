#ifndef DOMAIN_H
#define DOMAIN_H

//#if defined(_DEBUG) || defined(CHINESE_DEBUG)
#ifdef _DEBUG
#define _DBG_
#endif

#ifdef WIN32
#define BILL_WINDOWS
#endif

#ifndef WIN32
#define LINUS_LINUX
#endif

#ifdef CHINESE_DEBUG
#define TRUE_ONLY_WAY
#define GOOD_DEBUG_ERRORS
#endif

#define RESOURCES       MAIN_PATH   + "resources\\"
#define COMPILERS       RESOURCES   + "compilers\\"
#define EXAMPLES        RESOURCES   + "examples\\"
#define WORK_DIR        RESOURCES   + "working_directory\\"
#ifdef TRUE_ONLY_WAY
#define MAEDIA          RESOURCES   + "media\\"
#else
#define MAEDIA          MAIN_PATH   + "media\\"
#endif

#define MAIN_PATH       getMainPath()
#ifdef TRUE_ONLY_WAY
#define DB_PATH         RESOURCES   + "database\\"
#else
#define DB_PATH         getMainPath()
#endif
#define DATABASE_PATH DB_PATH + "db.sqlite3"

#define CHECKER_PATH    WORK_DIR    + "checker\\checker"
#define SOLUTION_PATH   WORK_DIR    + "solution\\solution"
#define TEST_PATH       WORK_DIR    + "tests\\"
#define ANSWERS_PATH    WORK_DIR    + "answers\\"
#define OUTPUT_PATH     WORK_DIR    + "outputs\\"
#define RESULT_PATH     WORK_DIR    + "results\\"

#define WINDOWS_OSS

#define _(aName) #aName

#define WLOG(aMessege)                                  \
    {                                                   \
        std::cout <<aMessege << "\n";                   \
    }

#define W_END_LOG                                       \
    {                                                   \
        std::cout << "\n";                              \
    }

#ifdef GOOD_DEBUG_ERRORS
#define WERROR(aNumber, aMessege)                       \
    {                                                   \
        std::cout << "\n\n\x1b[31m!!!!!!!!! "           \
            "ERROR HAS OCCURRED !!!!!!!!!\n\n"          \
            << "ERROR# " #aNumber "\n"                  \
            << aMessege << "\x1b[0m\n\n";               \
        exit(0);                                        \
    }
#else
#define WERROR(aNumber, aMessege)                       \
    {                                                   \
        std::cout << "\n\n\!!!!!!!!! "                  \
            "ERROR HAS OCCURRED !!!!!!!!!\n"            \
            << "ERROR# " #aNumber "\n"                  \
            << aMessege << "" << "\n\n" <<              \
            "\n\n";                                     \
        exit(0);                                        \
    }
#endif

#ifdef _DBG_
    #define WD_LOG(aMessege) WLOG(aMessege) 
    #define WD_END_LOG W_END_LOG 
    #define WD_ERROR(aNumber, aMessege) WERROR(aNumber, aMessege) 
#else
    #define WD_LOG(aMessege) 
    #define WD_END_LOG
    #define WD_ERROR(aNumber, aMessege)
#endif // DEBUG
// /*>*/

#define print(aMessage)         \
{                               \
    std::cout << aMessage;      \
}

typedef signed char		    sint_8;
typedef unsigned char	    uint_8;
typedef signed short	    sint_16;
typedef unsigned short	    uint_16;
typedef signed long		    sint_32;
typedef unsigned long	    uint_32;
typedef signed long	long	sint_64;
typedef unsigned long long	uint_64;

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
//#include <libloaderapi.h>
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
#endif //DOMAIN_H