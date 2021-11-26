#include <string>
#include <iostream>

#include "core.h"

int main(int argc, char* argv[])
{
    int num = 10;
    if (argc > 1)
    {
        num = std::stoi(getString(argv[1]));
    }
    WD_LOG("ID: " << num);
    WD_END_LOG;
    Core core(DATABASE_PATH);
    core.run(num);
    return 0;
}