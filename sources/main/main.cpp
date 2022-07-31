#include <string>

#include "tester/core.hpp"
#include "domain/error_message.hpp"

#include "path.hpp"

//--------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    int num = 3;
    if (argc > 1)
    {
        num = std::stoi(std::string(argv[1]));
    }

    WRITE_LOG("ID:", num);
    WRITE_LOG_ENDL();
    test::Core core(DATABASE_PATH);
    core.run(num);
    
    return 0;
}

//--------------------------------------------------------------------------------
