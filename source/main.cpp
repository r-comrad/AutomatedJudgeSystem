#include "core.h"

int main()
{
    Core core(DATABASE_PATH);
    core.run(10);
    return 0;
}