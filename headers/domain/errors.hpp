#ifndef ERRORS_HPP
#define ERRORS_HPP

//------------------------------------------------------------
//					ERROR MESSEGES DECLARATION
//------------------------------------------------------------

#include <iostream>
#include <string>

#include "domain/domain.hpp"
#include "domain/my_strings.hpp"

/*
\brief Write log information to output.
\param aMessege Log messege.
*/
#define W_LOG(aMessege)                                 \
    do {                                                \
        std::cout <<aMessege << "\n";                   \
    } while(0)

/*
\brief Write array to log output.
\param aMessege Aray for output.
*/
#define W_VECTOR_LOG(aMessege)                          \
    do {                                                \
        for(auto& i : aMessege)                         \
            std::cout << i << "\n";                     \
    } while (0)

/*
\brief Write array that contains pointers to log output.
\param aMessege Aray for output.
*/
#define W_VECTOR_PTR_LOG(aMessege)                      \
    do {                                                \
        for(auto& i : aMessege)                         \
            if (i != NULL)                              \
                std::cout << i << "\n";                 \
    } while (0)

/*
\brief Write empty line in log output.
*/
#define W_END_LOG                                       \
    do {                                                \
        std::cout << std::endl;                         \
    } while (0)

#ifdef  GOOD_DEBUG_ERRORS
/*
\brief Write error messege in log output with red text color.
\param aNumber Error name.
\param aMessege Errore messege.
*/
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
/*
\brief Write error messege in log output.
\param aNumber Error name.
\param aMessege Errore messege.
*/
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
/*
\brief Gets error of WinAPI.
\return String with error messege.
*/
str_val GetLastErrorAsString();
#endif // !BILL_WINDOWS

#endif // !ERRORS_HPP