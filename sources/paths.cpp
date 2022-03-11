#include "paths.hpp"

std_string
getMainPath() {
#ifdef BILL_WINDOWS
    CHAR buffer[MAX_PATH] = { 0 };
    uint_8 size = GetModuleFileNameA(NULL, buffer, MAX_PATH);
#ifdef TRUE_ONLY_WAY
    for (int i = 0; i < 2; ++i) while (buffer[--size] != L'\\');
#else
    for (int i = 0; i < 1; ++i) while (buffer[--size] != L'\\');
#endif
    buffer[size + 1] = 0;
    //return std::string(buffer).substr(0, size + 1);
    return getString(buffer);
#endif
#ifdef LINUS_LINUX
    char buf[PATH_MAX + 1];
    if (readlink("/proc/self/exe", buf, sizeof(buf) - 1) == -1)
        throw std::string("readlink() failed");
    std::string str(buf);
    int i = str.size() - 1;
    for (int j = 0; j < 1; --i)
    {
        if (str[i] == '/') ++j;
    }
    return str.substr(0, i + 2);
    //return str.substr(0, str.rfind('/'));
#endif
}