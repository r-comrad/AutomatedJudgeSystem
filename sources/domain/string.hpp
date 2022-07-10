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

// typedef std::string			str_val;
// typedef std::string&		str_ref;
// typedef const std::string&	str_const_ref;

// using CharArray = std::unique_ptr<char[]>;
// using CharArrayTable = std::vector<std::unique_ptr<char[]>>;

// // #define str_val             std::string
// // #define str_ref             std::string&
// // #define str_const_ref const std::string&

// #define newStrCopy(str) \
// std::strcpy(new char[str.size() + 1], str.c_str())

// #define newCharPtrCopy(str) \
// std::strcpy(new char[strlen(str) + 1], str)

// #define newLargerStrCopy(str, offset) \
// std::strcpy(new char[str.size() + offset + 1], str.c_str())

// #define strCopy(strTo, strFrom) std::strcat(strTo, strFrom)

// //TODO:
// // str_val getString(const char* aString, bool aCheckZero = true);
// std::string getString(const unsigned char* aString) noexcept;
// std::string getString(const StringTable& aStrArray) noexcept;
// CharArray getCharArray(const StringTable& aStrArray) noexcept;
// CharArray getCharArray(const unsigned char* aArray) noexcept;

// #ifdef BILL_WINDOWS
// void makeWindowString(str_ref aString);
// #endif // !BILL_WINDOWS

//TODO: move operations!
namespace dom
{
    using CharArray = std::unique_ptr<char[]>;
    using CharArrayTable = std::vector<std::unique_ptr<char[]>>;

	class String
	{
	public:
		String() noexcept;
		String(         const char* aStr)   noexcept;
		String(const unsigned char* aStr)   noexcept;
		String( const std::string&  aStr)   noexcept;
		String(       std::string&& aStr)   noexcept;

		String(    const String&  aOther)   noexcept = delete;
		String(          String&& aOther)   noexcept = default;

        String(const CharArray&  aStr, size_t aNum) noexcept;
        String(const CharArray&  aStr, char aLetter) noexcept;

		~String() = default;
//--------------------------------------------------------------------------------
		String& operator=	(           const char* aStr)   noexcept;
		String& operator=	(  const unsigned char* aStr)   noexcept;
		String& operator=	(   const std::string&	aStr)   noexcept;
		String& operator=	(         std::string&& aStr)   noexcept;
		String& operator=	(      const String&  aOther)   noexcept = delete;
		String& operator=	(            String&& aOther)   noexcept = default;

		String& operator+	(           const char* aStr)   noexcept;
		String& operator+	(  const unsigned char* aStr)   noexcept;
		String& operator+	(   const std::string&  aStr)   noexcept;
		String& operator+	(         std::string&& aStr)   noexcept;

		void    operator+=	(           const char* aStr)   noexcept;
		void    operator+=	(  const unsigned char* aStr)   noexcept;
		void    operator+=	(   const std::string&  aStr)   noexcept;
		void    operator+=	(         std::string&& aStr)   noexcept;

		void merge() noexcept;

		void switchToCharArray()    noexcept;
		void switchToString()       noexcept;

		operator CharArray()    const noexcept;
		operator std::string()  const noexcept;

		CharArray	harvestCharArray()  noexcept;
		std::string harvestString()     noexcept;

        friend std::ostream& operator<<(std::ostream& os, const dom::String& aStr);

	private:
		enum class StrType { NonDetermin, CharArray, String };

		StrType mType;
		CharArrayTable mCharData;
		std::vector<std::string> mStrData;

		auto getSizeLumbda(const char* str)         const noexcept;
		auto getSizeLumbda(const CharArray& str)    const noexcept;
		auto getSizeLumbda(const std::string& str)  const noexcept;

		//auto getEmplaceLumbda(const CharArray& str) const noexcept;
		//auto getEmplaceLumbda(const std::string& str) const noexcept;

		auto getConstructorLumbda(const char* str)          const noexcept;
		auto getConstructorLumbda(const CharArray& str)     const noexcept;
		auto getConstructorLumbda(const std::string& str)   const noexcept;

		void clear(StrType aType = StrType::NonDetermin) noexcept;

		template <typename T>
		void add(T&& aStr)
		{
			if (mType != StrType::CharArray)
			{
				//copyToVector(mStrData, std::forward(aStr));
				copyToVector(mStrData,aStr);
			}
			else
			{
				//copyToVector(mCharData, std::forward(aStr));
				copyToVector(mCharData, aStr);
			}
		}

		template <typename T>
		size_t countElements(T&& aArr) const noexcept
		{
			size_t result = 0;
			if (aArr.size() > 0)
			{
				auto sizeLumbda = getSizeLumbda(aArr[0]);
				for (auto& str : aArr) result += sizeLumbda(str);
			}
			return result;
		}

		template <typename To, typename From>
		//size_t copy(To&& aTo, From&& aFrom, auto sizeLumbda) const noexcept
		size_t copy(To&& aTo, From&& aFrom, size_t aNum) const noexcept
		{
			//size_t i = sizeLumbda(aTo);
			size_t j = 0;
			while (aFrom[j])
			{
				aTo[aNum++] = aFrom[j++];
			}
			aTo[aNum] = '\0';
			return aNum;
		}

		template <typename To, typename From>
		void copyFromVector(To&& aTo, From&& aFrom) const noexcept
		{
			auto constructorLumbda = getConstructorLumbda(aTo);
			aTo = constructorLumbda(countElements(aFrom));
			aTo[0] = '\0';

			//auto sizeLumbda = getSizeLumbda(aTo);
			//for (auto& str : aFrom) copy(aTo, str, sizeLumbda);
			size_t i = 0;
			for (auto& str : aFrom) i = copy(aTo, str, i);
		}

		template <typename To, typename From>
		void copyToVector(To&& aTo, From&& aFrom) const noexcept
		{
			aTo.resize(aTo.size() + 1);
			auto constructorLumbda = getConstructorLumbda(aTo[0]);
			auto sizeLumbda = getSizeLumbda(aFrom);
			aTo.back() = constructorLumbda(sizeLumbda(aFrom));
			//copy(aTo.back(), aFrom, [](auto& aStr) constexpr { return 0; });
			copy(aTo.back(), aFrom, 0);
		}
	};
}

#endif // !DOM_STRINGS_HPP