#ifndef DOMAIN_H
#define DOMAIN_H

#include <iostream>
#include <string>

typedef signed char		sint_8;
typedef unsigned char	uint_8;
typedef signed short	sint_16;
typedef unsigned short	uint_16;
typedef signed long		sint_32;
typedef unsigned long	uint_32;

#define WERROR(aNumber, aName, aMessege)                                    \
    {                                                                       \
        std::cout << "ERROR#" #aNumber "."                                  \
            " | " #aName ": can't" aMessege                                 \
            ", error code is " << GetLastError() << std::endl;              \
        exit(0);                                                            \
    }



#endif //DOMAIN_H
