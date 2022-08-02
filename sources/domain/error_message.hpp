#ifndef ERROR_MESSAGE_HPP
#define ERROR_MESSAGE_HPP

//--------------------------------------------------------------------------------

#include <fstream>

#include <iostream>

#include "string.hpp"

//--------------------------------------------------------------------------------

namespace dom
{
    class Message
    {
    public:
        static Message globalMessages;

        template<typename... Args>
        void startLogBlock(const Args&... args) noexcept
        {
            write(mLogStream, args...);
            ++mLogBlockCount;
        }

        template<typename... Args>
        void endLogBlock(const Args&... args) noexcept
        {
            write(mLogStream, args...);
            --mLogBlockCount;
        }

        template<typename... Args>
        void writeLog(const Args&... args) noexcept
        {
            write(mLogStream, args...);
        }

        template<typename... Args>
        void writeError(const Args&... args) noexcept
        {
            #ifdef ERRORS_TO_LOG_OUTPUT
            write(mErrorStream, "ERROR", args...);
            #else
            writeWithoutTabulation(mErrorStream, "ERROR", args...);
            #endif
        }

        void writeLogEndl() noexcept;   

        #ifdef BILL_WINDOWS
        /*
        \brief Gets error of WinAPI.
        \return String with error messege.
        */
        std::string GetLastWinAPIError() noexcept;
        #endif

    private:

        Message() noexcept;

        std::ostream* mLogStream;
        std::ostream* mErrorStream;

        template<typename... Args>
        void write(std::ostream* aStream, Args&&... args) noexcept
        {
            std::string str;
            for(int8_t i = 0; i < mLogBlockCount; ++i) str += '\t';
            basicWrite(aStream, str, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void writeWithoutTabulation(std::ostream* aStream, Args&&... args) noexcept
        {
            std::string str;
            basicWrite(aStream, str, std::forward<Args>(args)...);
        }

        template<typename... Args>
        void basicWrite(std::ostream* aStream, std::string& str, Args&&... args) noexcept
        {
            ((void) (str+= toString(std::forward<Args>(args)) + " ", 0), ...);
            str += '\n';

            (*aStream) << str ;
            (*aStream).flush();
        }

        template<typename S, typename = enableIfSame<S, std::string>>
        std::string toString(S&& str) noexcept
        {
            return std::forward<S>(str);
        }

        std::string toString(const char* str) noexcept
        {
            return std::string(str);
        }

        std::string toString(int64_t num) noexcept
        {
            return std::to_string(num);
        }

        int8_t mLogBlockCount;
    };
}

//--------------------------------------------------------------------------------

/*
ERROR FORMAT: <file or class name>, <function>, 
                <approximate error number>, <error message>
*/

#ifdef _DBG_
    #define START_LOG_BLOCK(...)    dom::Message::globalMessages.startLogBlock(__VA_ARGS__)
    #define END_LOG_BLOCK(...)      dom::Message::globalMessages.endLogBlock(__VA_ARGS__)
    #define WRITE_LOG(...)          dom::Message::globalMessages.writeLog(__VA_ARGS__)
    #define WRITE_LOG_ENDL(...)     dom::Message::globalMessages.writeLogEndl(__VA_ARGS__)
    #define WRITE_ERROR(...)        dom::Message::globalMessages.writeError(__VA_ARGS__)
#else
    #define START_LOG_BLOCK(...)    void(0)
    #define END_LOG_BLOCK(...)      void(0)
    #define WRITE_LOG(...)          void(0)
    #define WRITE_LOG_ENDL(...)     void(0) 
    #define WRITE_ERROR(...)        void(0)    
#endif 

//--------------------------------------------------------------------------------

#endif // !ERROR_MESSAGE_HPP
