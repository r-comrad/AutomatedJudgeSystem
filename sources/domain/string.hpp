#ifndef DOM_STRINGS_HPP
#define DOM_STRINGS_HPP

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#undef _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <list>
#include <memory>
#include <iostream>
#include <optional>

#include "flags.hpp"
#include "type.hpp"
#include "pair.hpp"

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
        enum class StrType { NonDetermin, CharArray, String };

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

        bool    operator==	(           const char* aStr)   const noexcept;
        bool    operator==	(   const std::string&  aStr)   const noexcept;
        bool    operator==	(   const dom::String&  aStr)   const noexcept;

        void merge() noexcept;

        void switchToCharArray()        noexcept;
        void switchToString()           noexcept;

        operator CharArray()      const noexcept;
        operator std::string()    const noexcept;

        CharArray	harvestCharArray()  noexcept;
        std::string harvestString()     noexcept;

        friend String operator+(const char* lhs, String&& rhs) noexcept;
        friend String operator+(const std::string& lhs, String&& rhs) noexcept;
        //friend String operator+(dom::String&& lhs, const dom::String& rhs) noexcept;
        //friend String operator+(dom::String&& lhs, const std::string& rhs) noexcept;
        //friend String operator+(dom::String&& lhs, const char* rhs) noexcept;
        friend std::ostream& operator<<(std::ostream& os, const dom::String& aStr) noexcept;

        bool isEmpty() const noexcept;
        size_t getSize() const noexcept;

        void pushFront(const char* aStr) noexcept;
        void pushFront(const std::string&  aStr) noexcept;
        void pushFront(std::string&& aStr) noexcept;

        
        const char* getFront() const noexcept;

    private:
        struct FrontResult
        {
            dom::String* mThis;

            operator char*()
            {
                return mThis->mCharData.front().get();
            }
            operator const char*()
            {
                if (mThis->mType != StrType::CharArray)
                {
                    return mThis->mStrData.front().c_str();
                }
                else
                {
                    return mThis->mCharData.front().get();
                }

            }
            operator std::string&()
            {
                return mThis->mStrData.front();
            }
        };

    public:
        template <typename T>
        T getFront() noexcept
        {
            return T(FrontResult(this));
        }

        std::optional <std::string> backSubStr(char aDelimiter) const noexcept;

        String getCopy(String::StrType aType = String::StrType::NonDetermin) const noexcept;

    private:
        StrType mType;
        std::list<std::unique_ptr<char[]>> mCharData;
        std::list<std::string> mStrData;

        auto getSize(const char* str)         const noexcept;
        auto getSize(const CharArray& str)    const noexcept;
        auto getSize(const std::string& str)  const noexcept;

        auto construct(const char* str, size_t aSize)          const noexcept;
        auto construct(const CharArray& str, size_t aSize)     const noexcept;
        auto construct(const std::string& str, size_t aSize)   const noexcept;
        auto construct(const std::list<std::unique_ptr<char[]>>& str, size_t aSize) const noexcept;
        auto construct(const std::list<std::string>& str, size_t aSize)   const noexcept;

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
        void copyFromVector(To& aTo, From&& aFrom) const noexcept
        {
            aTo = construct(aTo, countElements(aFrom));
            aTo[0] = '\0';

            size_t i = 0;
            for (auto& str : aFrom) i = copy(aTo, str, i);
        }

        template <typename To, typename From>
        void copyToVector(To&& aTo, From&& aFrom, bool aBackdoorInsert = true) const noexcept
        {
            auto temp = construct(aTo, getSize(aFrom));
            if (aBackdoorInsert) 
            {
                //TODO: do i need move?
                aTo.emplace_back(std::move(temp));  
                copy(aTo.back(), aFrom, 0);
            }
            else
            {
                //TODO: do i need move?
                aTo.emplace_front(std::move(temp));    
                copy(aTo.front(), aFrom, 0);
            }
        }

        template <typename T1, typename T2>
        bool compare(const T1& lhs, const T2& rhs) const noexcept
        {
            auto it = lhs.begin();
            size_t i = 0, j = 0;
            while(rhs[j])
            {
                if ((*it)[i++] != rhs[j++]) return false;
                if ((*it)[i] == '\0') 
                {
                    ++it;
                    i = 0;
                }
                if (it == lhs.end()) return rhs[j] == '\0';
            }
            return false;
        }


        //
        //template <typename T>
        //auto backFindgg(const T& aStr, char aChar) const noexcept

        template <typename T>
        struct Slice
        {
            T::const_iterator it;
            size_t ind;
            size_t size;
        };

        //std::optional <dom::Pair<decltype(std::begin(T())), size_t>> 
        template <typename T>
        std::optional <Slice<T>> 
        backFindg(const T& aStr, char aChar) const noexcept
        {  
            Slice<T> pos {aStr.end(), getSize(aStr.back()), 0};       

            do {
                pos.it--;
                do {
                    pos.ind--;
                    pos.size++;
                    if ((*pos.it)[pos.ind] == aChar) 
                    {
                        return pos;
                    }
                }while(pos.ind >= 0);
            } while(pos.it != aStr.begin());

            return {};
        }

        template <typename T>
        std::optional <std::string>
        backSubStr(const T& aStr, char aChar) const noexcept
        {     
            std::optional <std::string> result;
            auto slice = backFindg(aStr, aChar);
            if (slice.has_value())
            {
                std::string resStr; 
                resStr.reserve(slice.value().size - 1);

                size_t num = slice.value().ind + 1;
                while((*slice.value().it)[num])
                {
                    resStr.push_back((*slice.value().it)[num++]);
                }

                slice.value().it++;
                for(auto it = slice.value().it; it != aStr.end(); ++it)
                {
                    num = 0;
                    while((*it)[num])
                    {
                        resStr.push_back((*it)[num]);
                    }
                }
                result = resStr;
            }

            return result;
        }

        // template <typename T1, typename T2>
        // bool compareTwoClasses(T1&& lhs, T2&& rhs) const noexcept
        // {
        //     auto it1 = lhs.begin();
        //     auto it2 = rhs.begin();
        //     size_t i = 0, j = 0;
        //     while(rhs[j])
        //     {
        //         if (*(it)[i++] != rhs[j++]) return false;
        //         if (*(it)[i] == '\0') 
        //         {
        //             ++it;
        //             i = 0;
        //         }
        //         if (it == lhs.end()) return rhs[j] == '\0';
        //     }
        //     return false;
        // }
    };

    using StringTable = std::vector<String>;
}

#endif // !DOM_STRINGS_HPP