#include "domain/path.hpp"

#if     defined(BILL_WINDOWS)

#define CHINA_RESOURCES EXECUTABLE_PATH + "resources/"
#define COMPILERS       CHINA_RESOURCES + "compilers\\"
#define EXAMPLES        CHINA_RESOURCES + "examples\\"
#define WORK_DIR        CHINA_RESOURCES + "working_directory\\"
#define MAEDIA          CHINA_RESOURCES + "media\\"

#define DB_PATH         EXECUTABLE_PATH   
#define DATABASE_PATH   DB_PATH     + "db.sqlite3"

#define CHECKER_PATH    WORK_DIR    + "checker\\checker"
#define SOLUTION_PATH   WORK_DIR    + "solution\\solution"
#define TEST_PATH       WORK_DIR    + "tests\\"
#define ANSWERS_PATH    WORK_DIR    + "answers\\"
#define RESULT_PATH     WORK_DIR    + "results\\"

// !BILL_WINDOWS
#elif   defined(LINUS_LINUX)

#define CHINA_RESOURCES EXECUTABLE_PATH + "resources/"
#define COMPILERS       RESOURCES   + "compilers/"
#define EXAMPLES        RESOURCES   + "examples/"
#define WORK_DIR        RESOURCES   + "working_directory/"

#define MAEDIA          MAIN_PATH   + "media/"

#define DB_PATH         EXECUTABLE_PATH
#define DATABASE_PATH   DB_PATH     + "db.sqlite3"

#define CHECKER_PATH    WORK_DIR    + "checker/checker"
#define SOLUTION_PATH   WORK_DIR    + "solution/solution"
#define TEST_PATH       WORK_DIR    + "tests/"
#define ANSWERS_PATH    WORK_DIR    + "answers/"
#define OUTPUT_PATH     WORK_DIR    + "outputs/"
#define RESULT_PATH     WORK_DIR    + "results/"
#endif // !LINUS_LINUX

#define CPP_COMPILER_NAME   COMPILERS +   "magicCPPCompiler.cmd"