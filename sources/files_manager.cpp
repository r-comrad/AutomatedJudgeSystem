#include "files_manager.hpp"

void
copyFile(
    std_string_ref aFromFileName,
    std_string_ref aToFileName
)
{
    WD_LOG("Copying file\nFrom : " << aFromFileName);
    WD_LOG("To   : " << aToFileName);
    std::ifstream fromFile(aFromFileName);
    std::ofstream toFile(aToFileName);
    std::string s;
    while (std::getline(fromFile, s, '\0')) toFile << s << '\n';
    WD_END_LOG;
}