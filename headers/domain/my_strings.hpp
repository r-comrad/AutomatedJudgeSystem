#ifndef MY_STRINGS_HPP
#define MY_STRINGS_HPP

#define _CRT_SECURE_NO_WARNINGS

#include <string>

#include "domain/domain.hpp"

//typedef std::string         std_string;
//typedef char                std_string_cell;
//typedef std_string&			std_string_ref;

typedef std::string			str_val;
typedef std::string&		str_ref;
typedef const std::string&	str_const_ref;
typedef char                std_string_cell;


#define newStrCopy(str) \
std::strcpy(new char[str.size() + 1], str.c_str())

#define newCharPtrCopy(str) \
std::strcpy(new char[strlen(str) + 1], str)

#define newLargerStrCopy(str, offset) \
std::strcpy(new char[str.size() + offset + 1], str.c_str())

#define strCopy(strTo, strFrom) std::strcat(strTo, strFrom)

str_val getString(const char* aString, bool aCheckZero = true);
str_val getString(const unsigned char* aString, bool aCheckZero = true);

#ifdef BILL_WINDOWS
void makeWindowString(str_ref aString);
#endif // !BILL_WINDOWS

#endif // !MY_STRINGS_HPP