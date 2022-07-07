#ifndef DOM_STRINGS_HPP
#define DOM_STRINGS_HPP

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#undef _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <memory>

#include "flags.hpp"
#include "type.hpp"

using namespace std::literals;

typedef std::string			str_val;
typedef std::string&		str_ref;
typedef const std::string&	str_const_ref;

using CharArray = std::unique_ptr<char[]>;
using StringTable = std::vector<std::unique_ptr<char[]>>;

// #define str_val             std::string
// #define str_ref             std::string&
// #define str_const_ref const std::string&

#define newStrCopy(str) \
std::strcpy(new char[str.size() + 1], str.c_str())

#define newCharPtrCopy(str) \
std::strcpy(new char[strlen(str) + 1], str)

#define newLargerStrCopy(str, offset) \
std::strcpy(new char[str.size() + offset + 1], str.c_str())

#define strCopy(strTo, strFrom) std::strcat(strTo, strFrom)

//TODO:
// str_val getString(const char* aString, bool aCheckZero = true);
std::string getString(const unsigned char* aString) noexcept;
std::string getString(const StringTable& aStrArray) noexcept;
CharArray getCharArray(const StringTable& aStrArray) noexcept;
CharArray getCharArray(const unsigned char* aArray) noexcept;

#ifdef BILL_WINDOWS
void makeWindowString(str_ref aString);
#endif // !BILL_WINDOWS

namespace dom
{
	class String
	{
		String(const char* aStr) noexcept;
		String(const unsigned char* aStr) noexcept;
		String(const std::string& aStr) noexcept;
		String(std::string&& aStr) noexcept;
		
	private:
		enum class StrType { NonDetermin, CharArray, String};

		StrType mType;
		StringTable mCharData;
		std::vector<std::string> mStrData;

		void add(const char* aStr) noexcept;
		void add(const unsigned char* aStr) noexcept;
		void add(const std::string& aStr) noexcept;
		void add(std::string&& aStr) noexcept;

		void addCharToChar(const char* aStr) noexcept;
		void addStrToStr(const std::string& aStr) noexcept;
		void moveStrToStr(std::string&& aStr) noexcept;	
		void addStrToChar(const std::string& aStr) noexcept;
		
		void addUnsignedToChar(const unsigned char* aStr) noexcept;
		void addUnsignedToStr(const unsigned char* aStr) noexcept;

		void operator+= (const char* aStr) noexcept;
		void operator+= (const unsigned char* aStr) noexcept;
		void operator+= (const std::string& aStr) noexcept;
		void operator+= (std::string&& aStr) noexcept;

		void merge() noexcept;

		operator std::string();
		operator std::string();
		operator std::string();
	};
}

#endif // !DOM_STRINGS_HPP