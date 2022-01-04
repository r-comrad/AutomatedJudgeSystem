#include <string>
#include <iostream>

#include "core.h"
int main(int argc, char* argv[])
{
    //std::wcout << std::endl;
    //std::wcout << MAIN_PATH;
    //std::wcout << std::endl;
    //std::wcout << RESOURCES;
    //return 0;
    int num = 3;
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