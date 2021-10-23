#include "domain.h"

std::wstring getMainPath() {
#ifdef WINDOWS_OSS
    TCHAR buffer[MAX_PATH] = { 0 };
    uint_8 size = GetModuleFileName(NULL, buffer, MAX_PATH);
    for (int i = 0; i < 2; ++i) while (buffer[--size] != L'\\');
    return std::wstring(buffer).substr(0, size + 1);
#endif
}

std::string makeGoodString(std::wstring aBadString)
{
    std::string goodString;
    for (auto& i : aBadString) goodString.push_back(char(i));
    return goodString;
}

std::wstring makeBadString(std::string aGoodString)
{
    std::wstring badString;
    for (auto& i : aGoodString) badString.push_back(char(i));
    return badString;
}

