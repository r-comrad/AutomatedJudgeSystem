#ifndef PATHS_HPP
#define PATHS_HPP

#ifdef LINUS_LINUX
#include <unistd.h>
#endif

#include "domain.h"
#include "my_strings.hpp"

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
// !BILL_WINDOWS
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
#endif // !LINUS_LINUX

#define CPP_COMPILER_NAME   COMPILERS +   "magicCPPCompiler.cmd"

std_string getMainPath();

#endif // !PATHS_HPP