#include "my_strings.hpp"

std_string
getString
(
    const char* aString,
    bool aCheckZero
)
{
    std_string result(aString);
    if (aCheckZero && result[result.size()] != 0)
    {
        // TODO: check for zero index
        // TODO1
        result.push_back(0);
    }
    return result;
}

std_string
getString
(
    const unsigned char* aString,
    bool aCheckZero
)
{
    std_string result;// (aString);
    for (int i = 0; aString[i]; ++i) result.push_back(aString[i]);
    //if (aCheckZero && result[result.size()] != 0)
    //{
    //    // TODO: check for zero index
    //    // TODO1
    //    result.push_back(0);
    //}
    return result;
}

#ifdef BILL_WINDOWS
void
makeWindowString
(
    std_string_ref aString
)
{
    for (std_string_cell& i : aString)
    {
        if (i == '/') i = '\\';
    }
}
#endif // !BILL_WINDOWS