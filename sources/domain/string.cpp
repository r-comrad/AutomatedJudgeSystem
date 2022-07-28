#include "string.hpp"


dom::CharArray::CharArray() noexcept :
    mSize       (0),
    mCapacity   (0)
{
    reserve(64);
}

dom::CharArray::CharArray(const CharArrayTable& aStr) noexcept :    
    mSize       (0),
    mCapacity   (0)
{
    size_t size = 0;
    for(const auto& str : aStr) size += str.getSize();
    reserve(size);
    for(const auto& str : aStr) add(str);
}

void 
dom::CharArray::reserve(size_t aSize) noexcept
{
    if (aSize < 64) aSize = 64;
    if (mCapacity == 0) mCapacity = 1;

    if (aSize > mCapacity)
    {
        while (aSize >= mCapacity) mCapacity *= 2;
        std::unique_ptr<char[]> temp = std::make_unique<char[]>(mCapacity);
        copyArray(temp, mData);
        mData = std::move(temp);
    }
}

//------------------------------------------------------

dom::CharArray::operator char*() noexcept
{
    return mData.get();
}

dom::CharArray::operator const char*() const noexcept
{
    return mData.get();
}

std::string
dom::CharArray::getString() const noexcept
{
    std::string result(mData.get());
    return result;
}


//------------------------------------------------------

// dom::String
// dom::operator+(const char* lhs, String&& rhs) noexcept
// {
//     rhs.pushFront(lhs);
//     return std::move(rhs);
// }

// dom::String
// dom::operator+(const std::string& lhs, String&& rhs) noexcept
// {
//     rhs.pushFront(lhs);
//     return std::move(rhs);
// }


std::ostream& 
dom::operator<<(std::ostream& os, const dom::String& aStr) noexcept
{
    for(size_t i = 0; aData[i]; ++i) os << aData[i] << "\n";
    return os;
}

bool
dom::String::isEmpty() const noexcept
{
    return mSize == 0;
}

size_t
dom::String::getSize() const noexcept
{
    return mSize;
}

//------------------------------------------------------

auto
dom::String::getSize(const char* str) const noexcept
{
    return strlen(str);
}

auto
dom::String::getSize(const std::string& str) const noexcept
{
    return str.size();
}

std::optional <std::string>
dom::CharArray::backSubStr(char aDelimiter) const noexcept
{
    std::optional <std::string> result;

    size_t pos = mSize - 1;
    while(pos > -1 && mData[pos--] != aDelimiter);

    if (pos > 0)
    {
        std::string temp;
        temp.reserve(mSize - pos);
        for(size_t i = pos; i < mSize;++i) temp.push_back(mData[i]);
        result = temp;
    }

    return result;
}

dom::String
dom::String::getCopy(String::StrType aType) const noexcept
{
    dom::String result;
    result.mType = aType;
    if(result.mType == String::StrType::NonDetermin) result.mType = mType;

    if (result.mType != String::StrType::CharArray)
    {
        result.mStrData.resize(1);
        if (mType != String::StrType::CharArray)
        {         
            result.copyFromVector(result.mStrData.front(), mStrData);
        }
        else
        {
            result.mCharData.resize(1);
            result.copyFromVector(result.mStrData.front(), mCharData);
        }
    }
    else
    {
        result.mCharData.resize(1);
        if (mType != String::StrType::CharArray)
        {         
            result.copyFromVector(result.mCharData.front(), mStrData);
        }
        else
        {
            result.mCharData.resize(1);
            result.copyFromVector(result.mCharData.front(), mCharData);
        }
    }

    return result;
}

//------------------------------------------------------

auto
dom::String::construct(const char* str, size_t aSize) const noexcept
{
    return new char[aSize + 1];
}

auto
dom::String::construct(const CharArray& str, size_t aSize) const noexcept
{
    return std::make_unique<char[]>(aSize + 1);
}

auto 
dom::String::construct(const std::string& str, size_t aSize) const noexcept
{
    std::string result;
    result.resize(aSize);
    return result;
}

auto
dom::String::construct(const std::list<std::unique_ptr<char[]>>& str, size_t aSize) const noexcept
{
    return std::make_unique<char[]>(aSize + 1);
}

auto 
dom::String::construct(const std::list<std::string>& str, size_t aSize) const noexcept
{
    std::string result;
    result.resize(aSize);
    return result;
}

void
dom::String::clear(StrType aType) noexcept
{
    mType = aType;
    mCharData.resize(0);
    mStrData.resize(0);
}