#include "core.h"
#include <string>

int main()
{
    int num;

    std::wstring path = L"D:\\projects\\VS\\Tester\\Tester\\Resources\\";
    std::wstring programPath = path + L"NeoTest2.exe";
    std::wstring inputPath = path + L"input";
    std::wstring outputPath = path + L"output.b";

    Core core;
    core.runProcess(programPath, inputPath, outputPath);
}