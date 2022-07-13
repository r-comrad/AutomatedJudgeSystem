#include "domain/string.hpp"

dom::String::String() noexcept :
    mType(StrType::NonDetermin)
{}

dom::String::String(const char* aStr) noexcept :
    mType    (StrType::CharArray)
{
    add(aStr);
}

dom::String::String(const unsigned char* aStr) noexcept :
    String    ((const char*)aStr)
{}

dom::String::String(const std::string& aStr) noexcept :
    mType    (StrType::String)
{
    add(aStr);
}

dom::String::String(std::string&& aStr) noexcept :
    mType    (StrType::String)
{
    add(std::move(aStr));
}

dom::String::String(const CharArray&  aStr, size_t aNum) noexcept :
    mType    (StrType::CharArray)
{
    auto ptr = std::make_unique<char[]> (aNum + 1);
    ptr[aNum] = 0;
    add(ptr);
}

dom::String::String(const CharArray&  aStr, char aLetter) noexcept :
    mType    (StrType::CharArray)
{
    size_t i = 0;
    while(aStr[i] && aStr[i++] != aLetter);
    (*this) = String(aStr, i);
}

//------------------------------------------------------

dom::String&
dom::String::operator= (const char* aStr) noexcept
{
    clear(StrType::CharArray);
    add(aStr);
    return *this;
}

dom::String&
dom::String::operator= (const unsigned char* aStr) noexcept
{
    return this->operator+((const char*)aStr);
    return *this;
}

dom::String&
dom::String::operator= (const std::string& aStr) noexcept
{
    clear(StrType::String);
    add(aStr);
    return *this;
}

dom::String&
dom::String::operator= (std::string&& aStr) noexcept
{
    clear(StrType::String);
    add(std::move(aStr));
    return *this;
}

//------------------------------------------------------

dom::String&
dom::String::operator+ (const char* aStr) noexcept
{
    add(aStr);
    return *this;
}

dom::String&
dom::String::operator+ (const unsigned char* aStr) noexcept
{
    return this->operator+((const char*)aStr);
    return *this;
}

dom::String&
dom::String::operator+ (const std::string& aStr) noexcept
{
    add(aStr);
    return *this;
}

dom::String&
dom::String::operator+ (std::string&& aStr) noexcept
{
    add(std::move(aStr));
    return *this;
}

//------------------------------------------------------

void
dom::String::operator+= (const char* aStr) noexcept
{
    add(aStr);
}

void
dom::String::operator+= (const unsigned char* aStr) noexcept
{
    this->operator+=((const char*)aStr);
}

void
dom::String::operator+= (const std::string& aStr) noexcept
{
    add(aStr);
}

void
dom::String::operator+= (std::string&& aStr) noexcept
{
    add(std::move(aStr));
}

//------------------------------------------------------

void
dom::String::merge() noexcept
{
    if (mType != StrType::CharArray)
    {
        std::string str;
        copyFromVector(str, mStrData);
        mStrData.resize(1);
        mStrData[0] = std::move(str);
    }
    else
    {
        CharArray str;
        copyFromVector(str, mCharData);
        mCharData.resize(1);
        mCharData[0] = std::move(str);
    }
}

//------------------------------------------------------

void
dom::String::switchToCharArray() noexcept
{
    mType = StrType::CharArray;
}

void
dom::String::switchToString() noexcept
{
    mType = StrType::String;
}

//------------------------------------------------------

dom::String::operator dom::CharArray() const noexcept
{
    CharArray result;

    if (mType != StrType::CharArray)
    {
        copyFromVector(result, mStrData);
    }
    else
    {
        copyFromVector(result, mCharData);
    }

    return result;
}

dom::String::operator std::string() const noexcept
{
    std::string result;

    if (mType != StrType::CharArray)
    {
        copyFromVector(result, mStrData);
    }
    else
    {
        copyFromVector(result, mCharData);
    }

    return result;
}

dom::CharArray
dom::String::harvestCharArray() noexcept
{
    CharArray result {nullptr};

    merge();
    if (mType != StrType::CharArray)
    {
        copyFromVector(result, mStrData);
    }
    else if (mCharData.size() > 0)
    {
        result = std::move(mCharData[0]);
    }

    return result;
}

std::string
dom::String::harvestString() noexcept
{
    std::string result;

    merge();
    if (mType != StrType::CharArray)
    {
        result = std::move(mStrData[0]);
    }
    else if (mCharData.size() > 0)
    {
        copyFromVector(result, mCharData);
        
    }

    return result;
}

std::ostream& 
dom::operator<<(std::ostream& os, const dom::String& aStr)
{
    if (aStr.mType != String::StrType::CharArray)
    {
        for(const auto& str : aStr.mStrData) os << str << "\n";
    }
    else
    {
        for(const auto& str : aStr.mCharData) os << str.get() << "\n";
    }
    return os;
}

bool
dom::String::isEmpty() const noexcept
{
    return getSize() == 0;
}

size_t
dom::String::getSize() const noexcept
{
    size_t result;
    if (mType != String::StrType::CharArray)
    {
        result = countElements(mStrData);
    }
    else
    {
        result = countElements(mCharData);
    }
    return result;
}

//------------------------------------------------------

auto
dom::String::getSize(const char* str) const noexcept
{
    return strlen(str);
}

auto
dom::String::getSize(const CharArray& str) const noexcept
{
    return strlen(str.get());
}

auto
dom::String::getSize(const std::string& str) const noexcept
{
    return str.size();
}

auto
dom::String::construct(const char* str, size_t aSize) const noexcept
{
    return new char[aSize + 1];
}

auto
dom::String::construct(const CharArray& str, size_t aSize) const noexcept
{
    return std::move(std::make_unique<char[]>(aSize + 1));
}

auto 
dom::String::construct(const std::string& str, size_t aSize) const noexcept
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