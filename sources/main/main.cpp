#include <string>

#include "tester/core.hpp"
#include "domain/error_message.hpp"

#include "path.hpp"

//--------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    dom::Pair<int,int> a = {6,6};
    dom::Pair<int,int> b = {6,6};
    a+=b;
    a = a+b;
    a = std::move(a)+b;
    a = a+std::move(b);

    a = b - a;
    b += {6, 6};
    a = {6, 6};
    a = std::move(b) - a;


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
