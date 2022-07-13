#ifndef DOM_STRINGS_HPP
#define DOM_STRINGS_HPP

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#undef _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <memory>
#include <iostream>

#include "flags.hpp"
#include "type.hpp"

using namespace std::literals;

//TODO: move operations!
namespace dom
{
    using CharArray = std::unique_ptr<char[]>;
    using CharSharedArray = std::shared_ptr<char[]>;
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

        void switchToCharArray()        noexcept;
        void switchToString()           noexcept;

        operator CharArray()      const noexcept;
        operator std::string()    const noexcept;

        CharArray	harvestCharArray()  noexcept;
        std::string harvestString()     noexcept;

        friend std::ostream& operator<<(std::ostream& os, const dom::String& aStr);

        bool isEmpty() const noexcept;
        size_t getSize() const noexcept;

    private:
        enum class StrType { NonDetermin, CharArray, String };

        StrType mType;
        CharArrayTable mCharData;
        std::vector<std::string> mStrData;

        auto getSize(const char* str)         const noexcept;
        auto getSize(const CharArray& str)    const noexcept;
        auto getSize(const std::string& str)  const noexcept;

        auto construct(const char* str, size_t aSize)          const noexcept;
        auto construct(const CharArray& str, size_t aSize)     const noexcept;
        auto construct(const std::string& str, size_t aSize)   const noexcept;

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
                for (auto& str : aArr) result += getSize(str);
            }
            return result;
        }

        template <typename To, typename From>
        size_t copy(To&& aTo, From&& aFrom, size_t aNum) const noexcept
        {
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
            aTo = construct(aTo, countElements(aFrom));
            aTo[0] = '\0';

            size_t i = 0;
            for (auto& str : aFrom) i = copy(aTo, str, i);
        }

        template <typename To, typename From>
        void copyToVector(To&& aTo, From&& aFrom) const noexcept
        {
            aTo.resize(aTo.size() + 1);
            aTo.back() = construct(aTo[0], getSize(aFrom));
            copy(aTo.back(), aFrom, 0);
        }
    };

    using StringTable = std::vector<String>;
}

#endif // !DOM_STRINGS_HPP