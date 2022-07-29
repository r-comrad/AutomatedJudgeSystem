#include "error_message.hpp"

//--------------------------------------------------------------------------------

#include <iostream>
#include "path.hpp"

//--------------------------------------------------------------------------------

// The main/path.hpp file overrides the log and error paths for this project.
#include "main/path.hpp"

//--------------------------------------------------------------------------------
/*
** This section shows all available values for
** the LOGS_OUTPUT_TYPE flag / ERRORS_OUTPUT_TYPE flag
** and what these values represent.
** -------------------------------
** LOGS_OUTPUT_TYPE values:
**
** LOGS_DEFAULT_OUTPUT     = 0
** LOGS_TO_COUT_OUTPUT     = 1
** LOGS_TO_FILE_OUTPUT     = 2
**
** DEFAULT = LOGS_TO_COUT_OUTPUT
** -------------------------------
** ERRORS_OUTPUT_TYPE values:
**
** ERRORS_DEFAULT_OUTPUT    = 0
** ERRORS_TO_COUT_OUTPUT    = 1
** ERRORS_TO_FILE_OUTPUT    = 2
** ERRORS_TO_LOG_OUTPU      = 4
**
** DEFAULT = ERRORS_TO_COUT_OUTPUT
** -------------------------------
*/
//--------------------------------------------------------------------------------

#if     LOGS_OUTPUT_TYPE == 0
    #define LOGS_DEFAULT_OUTPUT
#elif   LOGS_OUTPUT_TYPE == 1
    #define LOGS_TO_COUT_OUTPUT
#elif   LOGS_OUTPUT_TYPE == 2
    #define LOGS_TO_FILE_OUTPUT
#else
    #define LOG_DEFAULT_OUTPUT
#endif

//--------------------------------------------------------------------------------

#if     ERRORS_OUTPUT_TYPE == 0
    #define ERRORS_DEFAULT_OUTPUT
#elif   ERRORS_OUTPUT_TYPE == 1
    #define ERRORS_TO_COUT_OUTPUT
#elif   ERRORS_OUTPUT_TYPE == 2
    #define ERRORS_TO_FILE_OUTPUT
#elif   ERRORS_OUTPUT_TYPE == 4
    #define ERRORS_TO_LOG_OUTPU
#else
    #define ERROR_DEFAULT_OUTPUT
#endif

//--------------------------------------------------------------------------------

dom::Message dom::Message::globalMessages;

//--------------------------------------------------------------------------------

dom::Message::Message() noexcept
{
    mLogBlockCount       =  0;

    #if     defined(LOGS_DEFAULT_OUTPUT)
        mLogStream = &std::cout;
    #elif   defined(LOGS_TO_COUT_OUTPUT)
        mLogStream = &std::cout;
    #elif   defined(LOGS_TO_FILE_OUTPUT)
        auto sss = LOGS_PATH + "out.log";
        mLogStream = new std::ofstream(LOGS_PATH + "out.log");
    #endif

    #if     defined(ERRORS_DEFAULT_OUTPUT)
        mErrorStream = &std::cout;
    #elif   defined(ERRORS_TO_COUT_OUTPUT)
        mErrorStream = &std::cout;
    #elif   defined(ERRORS_TO_FILE_OUTPUT)
        mErrorStream = new std::ofstream(ERRORS_PATH + "out.err");
    #elif   defined(ERRORS_TO_LOG_OUTPU)
        mErrorStream = mLogStream;
    #endif
}

//--------------------------------------------------------------------------------

void 
dom::Message::writeLogEndl() noexcept
{
    (*mLogStream) << '\n';
}

//--------------------------------------------------------------------------------

#ifdef BILL_WINDOWS
    #include <windows.h>
#endif

//--------------------------------------------------------------------------------

#ifdef BILL_WINDOWS
std::string
dom::Message::GetLastWinAPIError() noexcept
{
    std::string result;

    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) 
    {
        LPSTR messageBuffer = nullptr;

        /*
        **Ask Win32 to give us the string version of that message ID.
        ** The parameters we pass in, tell Win32 to create the buffer 
        ** that holds the message for us 
        ** (because we don't yet know how long the message string will be).
        */
        size_t size = 
            FormatMessageA
            (
                FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                    FORMAT_MESSAGE_FROM_SYSTEM | 
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, 
                errorMessageID, 
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                (LPSTR)&messageBuffer, 
                0, 
                NULL
            );

        //Copy the error message into a std::string.
        for (int i = 0; i < size; ++i)
        {
            result.push_back(messageBuffer[i]);
        }

        //Free the Win32's string's buffer.
        LocalFree(messageBuffer);
    }

    return result;
}
#endif // !BILL_WINDOWS

//--------------------------------------------------------------------------------
