#ifndef DOM_PATHS_HPP
#define DOM_PATHS_HPP

#include <fstream>

#include "type.hpp"
#include "string.hpp"

#if     defined(BILL_WINDOWS)
    #include <windows.h>
#elif   defined(LINUS_LINUX)
    #include <unistd.h>
    #include <limits.h>
#endif

#define MAIN_PATH       dom::Path::getMainPath()
#define EXECUTABLE_PATH dom::Path::getExecutablePath()

#if     defined(BILL_WINDOWS)
    #define RESOURCES       MAIN_PATH   + "resources\\"s
    #define OUTPUT_PATH     RESOURCES   + "output\\"s
    #define LOGS_PATH       RESOURCES   + "logs\\"s
    #define ERRORS_PATH     RESOURCES   + "errors\\"s
// !BILL_WINDOWS
#elif   defined(LINUS_LINUX)
    #define RESOURCES       MAIN_PATH   + "resources/"s
    #define OUTPUT_PATH     RESOURCES   + "output/"s
    #define LOGS_PATH       RESOURCES   + "logs/"s
    #define ERRORS_PATH     WORK_DIR    + "errors/"s
// !LINUS_LINUX
#else   // NUN
    #define RESOURCES       ""s
    #define OUTPUT_PATH     ""s
    #define LOGS_PATH       ""s
    #define LOGS_PATH       ""s
// !NUN
#endif

namespace dom
{
    class Path
    {
    public:
        static std::string getMainPath() noexcept;
        static std::string getExecutablePath() noexcept;

    private:
        static std::string getMainPathOnce() noexcept;
        static std::string getExecutablePathOnce() noexcept;
    };
}

#undef min
#undef max

#endif // !DOM_PATHS_HPP