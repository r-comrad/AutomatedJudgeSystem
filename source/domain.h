#ifndef DOMAIN_H
#define DOMAIN_H

#if defined(_DEBUG) || defined(CHINESE_DEBUG)
#define _DBG_
#endif

#define RESOURCES L"resources\\"

#define WINDOWS_OSS

#define _(aName) #aName

#define WERROR(aNumber, aName, aMessege)                                    \
    {                                                                       \
        std::cout << "ERROR#" #aNumber "."                                  \
            " | " #aName ": can't" aMessege                                 \
            ", error code is " << GetLastError() << std::endl;              \
        exit(0);                                                            \
    }

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
//std::string makeGoodString(std::wstring aSS);

#endif //DOMAIN_H