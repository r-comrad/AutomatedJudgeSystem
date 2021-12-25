#include "domain.h"

std::string 
getMainPath() {
#ifdef BILL_WINDOWS
    CHAR buffer[MAX_PATH] = { 0 };
    uint_8 size = GetModuleFileNameA(NULL, buffer, MAX_PATH);
#ifdef TRUE_ONLY_WAY
    for (int i = 0; i < 2; ++i) while (buffer[--size] != L'\\');
#else
    for (int i = 0; i < 1; ++i) while (buffer[--size] != L'\\');
#endif
    return std::string(buffer).substr(0, size + 1);
#else
#endif
return  
}

//std::string 
//makeGoodString
//(
//    std::wstring aBadString
//)
//{
//    std::string goodString;
//    for (auto& i : aBadString) goodString.push_back(char(i));
//    return goodString;
//}
//
//std::wstring 
//makeBadString
//(
//    std::string aGoodString
//)
//{
//    std::wstring badString;
//    for (auto& i : aGoodString) badString.push_back(char(i));
//    return badString;
//}

std::string 
GetLastErrorAsString()
{
    setlocale(LC_ALL, "Russian");

    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::string(); //No error message has been recorded
    }

    LPWSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    //std::wstring message(messageBuffer, size);
    std::string message;
    for (int i = 0; i < size; ++i)
    {
        message.push_back(messageBuffer[i]);
    }

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}

void 
copyFile(
    std::string aFromFileName, 
    std::string aToFileName
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

//void
//copyFile(
//    std::wstring aFromFileName, 
//    std::wstring aToFileName
//)
//{
//    copyFile(makeGoodString(aFromFileName), makeGoodString(aToFileName));
//}

std::string getString(const unsigned char* aString)
{
    std::string result;
    for (int i = 0; aString[i]; ++i) result.push_back(aString[i]);
    return result;
}

std::string
getString
(
    const char* aString
)
{
    std::string result;
    for (int i = 0; aString[i]; ++i) result.push_back(aString[i]);
    return result;
}

std::string 
makeWindowString
(
    std::string aString
)
{
    //for (int i = 0; i < aString.size(); ++i)
    //{
    //    aString.erase(aString.begin() + i);
    //    aString.insert(aString.begin() + i, '\\');
    //    aString.insert(aString.begin() + i, '\\');
    //}
    for (auto& i : aString)
    {
        if (i == '/') i = '\\';
    }
    return aString;

}

//std::string GetLastErrorAsString()
//{
//    LPTSTR errorText = NULL;
//
//    FormatMessage(
//        // use system message tables to retrieve error text
//        FORMAT_MESSAGE_FROM_SYSTEM
//        // allocate buffer on local heap for error text
//        | FORMAT_MESSAGE_ALLOCATE_BUFFER
//        // Important! will fail otherwise, since we're not 
//        // (and CANNOT) pass insertion parameters
//        | FORMAT_MESSAGE_IGNORE_INSERTS,
//        NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
//        GetLastError(),
//        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//        (LPTSTR)&errorText,  // output 
//        0, // minimum size for output buffer
//        NULL);   // arguments - see note 
//
//    if (NULL != errorText)
//    {
//        // ... do something with the string `errorText` - log it, display it to the user, etc.
//
//        // release memory allocated by FormatMessage()
//        LocalFree(errorText);
//        errorText = NULL;
//    }
//
//    int i = 0;
//    while (errorText[i])  std::cout << errorText[i++];
//
//    return "";
//}
