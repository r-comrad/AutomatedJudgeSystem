#ifndef DOM_STRINGS_HPP
#define DOM_STRINGS_HPP

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#undef _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <memory>
#include <optional>

#include "type.hpp"
#include "pair.hpp"

using namespace std::literals;

//TODO: move operations!
namespace dom
{
    class CharArray;
    using CharArrayTable = std::vector<CharArray>;

    class CharArray
    {
        template<class T1, class T2>
        using isSameWeek = std::is_same<T1, std::decay_t<T>>::value

        template<class T>
        using isString<class T> = isSameWeek<char*, T> || isSameWeek<std::string, T> || isSameWeek<unsigned char*, T>
        
        template<class T>
        using stringCheck =  typename = std::enable_if_t<isString<class T>>
        

    public:
        CharArray() noexcept;
        ~CharArray() = default;

        CharArray(const CharArray& other) noexcept = delete;
        CharArray& operator= (const CharArray& other) noexcept = delete;
        
        CharArray(CharArray&& other) noexcept = default;
        CharArray& operator= (CharArray&& other) noexcept = default;

        template<typename S, stringCheck<S>>
        CharArray(S&& aStr) noexcept : 
            mSize       (0),
            mCapacity   (0)
        {
            reserve(getSize(aStr));
            add(std::forward(aStr));
        }

        CharArray(const CharArrayTable& aStr) noexcept;

//--------------------------------------------------------------------------------
        
        template<typename S, stringCheck<S>>
        CharArray& operator=    (S&& aStr) noexcept
        {
            *this = std::move(CharArray(std::forward(aStr)));
        }

        // String& operator+    (           const char* aStr)   noexcept;
        // String& operator+    (  const unsigned char* aStr)   noexcept;
        // String& operator+    (   const std::string&  aStr)   noexcept;
        // String& operator+    (         std::string&& aStr)   noexcept;

        template<typename S, stringCheck<S>>
        void operator+= (S&& aStr) noexcept
        {
            add(std::forward(aStr));
        }

        template<typename S, stringCheck<S>>
        void operator== (S&& aStr) noexcept
        {
            size_t i = 0, j = 0;
            while (mData[i] && aStr[j])
            {
                if (mData[i++] != aStr[j++]) return false;
            }
            return true;
        }

        operator char*() noexcept;
        operator const char*() const noexcept;
        std::string getString() const noexcept;

        //friend String operator+(const char* lhs, String&& rhs) noexcept;
        //friend String operator+(const std::string& lhs, String&& rhs) noexcept;
        friend std::ostream& operator<<(std::ostream& os, const dom::String& aStr) noexcept;

        bool isEmpty() const noexcept;
        size_t getSize() const noexcept;
        
        template<typename S, stringCheck<S>>
        void pushFront(S&& aStr) noexcept
        {
            size_t combinedSize = getSize(aStr) + mSize;
            CharArray temp;
            temp.reserve(combinedSize);
            temp.add(aStr);
            temp.add(mData);
            mData = std::move(temp.mData);
        }

        std::optional <std::string> backSubStr(char aDelimiter) const noexcept;

        CharArray getCopy() const noexcept;

        void reserve(size_t aSize) noexcept; 

    private:
        std::unique_ptr<char[]> mData;
        size_t mCapacity; 
        size_t mSize; 

        template<typename S>
        void add(S&& aStr) noexcept;
        {
            reserve(mSize + getSize(aStr));
            copyArray(mData, aStr, mSize);
        }

        template<typename S1, typename S2>
        void copyArray(S1&& aTo, S2&& aFrom, size_t aStartInd) noexcept;
        {
            size_t j = 0;
            while (aFrom[j])
            {
                aTo[aStartInd++] = aFrom[j++];
            }
            aTo[aStartInd] = '\0';
        }

        auto getSize(const char* str)         const noexcept;
        auto getSize(const std::string& str)  const noexcept;
    };
}

#endif // !DOM_STRINGS_HPP