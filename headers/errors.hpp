#ifndef ERRORS_HPP
#define ERRORS_HPP

#include "domain.h"
#include "my_strings.hpp"

#define W_LOG(aMessege)                                 \
    do {                                                \
        std::cout <<aMessege << "\n";                   \
    } while(0)

#define W_VECTOR_LOG(aMessege)                          \
    do {                                                \
        for(auto& i : aMessege)                         \
            std::cout << i << "\n";                     \
    } while (0)

#define W_VECTOR_PTR_LOG(aMessege)                      \
    do {                                                \
        for(auto& i : aMessege)                         \
            if (i != NULL)                              \
                std::cout << i << "\n";                 \
    } while (0)


#define W_END_LOG                                       \
    do {                                                \
        std::cout << std::endl;                         \
    } while (0)

#ifdef  GOOD_DEBUG_ERRORS
#define W_ERROR(aNumber, aMessege)                      \
    do {                                                \
        std::cout << std::string("\n\n\x1b[31m!!!!!!!!!"\
            "ERROR HAS OCCURRED !!!!!!!!!\n"            \
            "ERROR# " #aNumber "\n") +                  \
            std::string(aMessege) +                     \
            std::string("\x1b[0m\n\n\n\n")              \
            << std::endl; exit(0);                      \
    } while (0)
#else
#define W_ERROR(aNumber, aMessege)                      \
    do {                                                \
        std::cout << std::string("\n\n!!!!!!!!! "       \
            "ERROR HAS OCCURRED !!!!!!!!!\n"            \
            "ERROR# " #aNumber "\n") +                  \
            std::string(aMessege) +                     \
            std::string("\n\n\n\n")                     \
            << std::endl; exit(0);                      \
    } while (0)
#endif // !GOOD_DEBUG_ERRORS

#ifdef _DBG_
#define WD_LOG(aMessege)            W_LOG(aMessege) 
#define WD_END_LOG                  W_END_LOG 
#define WD_ERROR(aNumber, aMessege) W_ERROR(aNumber, aMessege) 
#else
#define WD_LOG(aMessege)            do {} while (0)
#define WD_END_LOG                  do {} while (0)
#define WD_ERROR(aNumber, aMessege) do {} while (0)
#endif // DEBUG

#ifdef BILL_WINDOWS
std_string GetLastErrorAsString();
#endif // !BILL_WINDOWS

#endif // !ERRORS_HPP