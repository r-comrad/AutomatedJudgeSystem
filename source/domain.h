#ifndef DOMAIN_H
#define DOMAIN_H

#if defined(_DEBUG) || defined(CHINESE_DEBUG)
#define _DBG_
#endif

#define RESOURCES L"resources\\"

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

#define WERROR(aNumber, aMessege)                       \
    {                                                   \
        std::cout << "\n\n\x1b[31m!!!!!!!!! "           \
            "ERROR HAS OCCURRED !!!!!!!!!\n\n"          \
            << "ERROR# " #aNumber "\n"                  \
            << aMessege << "\x1b[0m\n\n";               \
        exit(0);                                        \
    }

#ifdef _DBG_
    #define WD_LOG(aMessege) WLOG(aMessege) 
    #define WD_END_LOG W_END_LOG 
    #define WD_ERROR(aNumber, aMessege) WERROR(aNumber, aMessege) 
#else
    #define WDLOG(aMessege)
    #define WD_END_LOG
    #define WDERROR(aNumber, aMessege)
#endif // DEBUG

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
#include <windows.h>

std::wstring getMainPath();

std::string makeGoodString(std::wstring aBadString);
std::wstring makeBadString(std::string aGoodString);
//std::string makeGoodString(std::wstring aSS);

#endif //DOMAIN_H