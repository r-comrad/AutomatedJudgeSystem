#include "domain/string.hpp"

// str_val
// getString
// (
//     const char* aString,
//     bool aCheckZero
// )
// {
//     str_val result(aString);
//     if (aCheckZero && result[result.size()] != 0)
//     {
//         // TODO: check for zero index
//         // TODO1
//         result.push_back(0);
//     }
//     return result;
// }

std::string
getString
(
    const unsigned char* aString
) noexcept
{
    str_val result;
    for (int i = 0; aString[i]; ++i) result.push_back(aString[i]);
    return result;
}

std::string 
getString(const StringTable& aStrArray) noexcept
{
	std::string result;

	int size = 0;
	for(auto& str : aStrArray) size += strlen(str.get());
	result.reserve(size + 1);

	for(auto& str : aStrArray) 
	{
		result.append(str.get());
	}

	return result;
}

std::unique_ptr<char[]>
getCharArray(const StringTable& aStrArray) noexcept
{
	std::unique_ptr<char[]> result;

	int size = 0;
	for(auto& str : aStrArray) size += strlen(str.get());
	result.reset(new char[size + 1]);

	for(auto& str : aStrArray) 
	{
		strcat(result.get(), str.get());
	}

	return result;
}

CharArray
getCharArray
(
    const unsigned char* aString
) noexcept
{
    int size = 0;
	while (aString[size++]);
	CharArray result {new char[size]};
    for (int i = 0;i < size; ++i) 
	{
		result[i] = aString[i];
	}
    return result;
}

#ifdef BILL_WINDOWS
void
makeWindowString
(
    str_ref aString
)
{
    for (char& i : aString)
    {
        if (i == '/') i = '\\';
    }
}

#endif // !BILL_WINDOWS

dom::String::String(const char* aStr) noexcept
{
	add(aStr);
}

dom::String::String(const unsigned char* aStr) noexcept
{
	add(aStr);
}

dom::String::String(const std::string& aStr) noexcept
{
	add(aStr);
}

dom::String::String(std::string&& aStr) noexcept
{
	add(std::move(aStr));
}

//------------------------------------------------------

void
dom::String::add(const char* aStr) noexcept
{
	if (mType != StrType::CharArray)
	{
		moveStrToStr(std::string(aStr));
	}
	else
	{
		addCharToChar(aStr);
	}
}

void
dom::String::add(const unsigned char* aStr) noexcept
{
	if (mType != StrType::CharArray)
	{
		addUnsignedToStr(aStr);
	}
	else
	{
		addUnsignedToChar(aStr);
	}
}

void
dom::String::add(const std::string& aStr) noexcept
{
	if (mType != StrType::CharArray)
	{
		addStrToStr(aStr);
	}
	else
	{
		addStrToChar(aStr);
	}
}

void
dom::String::add(std::string&& aStr) noexcept
{
	if (mType != StrType::CharArray)
	{
		moveStrToStr(std::move(aStr));
	}
	else
	{
		addStrToChar(aStr);
	}
}

//------------------------------------------------------

void
dom::String::addCharToChar(const char* aStr) noexcept
{
	mCharData.resize(mCharData.size() + 1);
	size_t size = strlen(aStr);
	mCharData.back() = std::make_unique<char[]>(size + 1);
	std::strcat(mCharData.back().get(), aStr);
}

void
dom::String::addStrToStr(const std::string& aStr) noexcept
{
	mStrData.resize(mStrData.size() + 1);
	mStrData.back() = aStr;
}

void
dom::String::moveStrToStr(std::string&& aStr) noexcept
{
	mStrData.resize(mStrData.size() + 1);
	mStrData.back() = std::move(aStr);
}

void
dom::String::addStrToChar(const std::string& aStr) noexcept
{
	mCharData.resize(mCharData.size() + 1);
	mCharData.back() = std::make_unique<char[]>(aStr.size() + 1);
	for(size_t i = 0; i < aStr.size(); ++i) mCharData.back()[i] = aStr[i];
	mCharData.back()[aStr.size()] = 0;
}

//------------------------------------------------------

void
dom::String::addUnsignedToChar(const unsigned char* aStr) noexcept
{
	mCharData.resize(mCharData.size() + 1);
	size_t size = 0;
	while(aStr[size++]);
	mCharData.back() = std::make_unique<char[]>(size);
	for(size_t i = 0; i < size - 1; ++i) mCharData.back()[i] = aStr[i];
	mCharData.back()[size - 1] = 0;
}

void
dom::String::addUnsignedToStr(const unsigned char* aStr) noexcept
{
	mStrData.resize(mStrData.size() + 1);
	size_t size = 0;
	while(aStr[size++]);
	mStrData.reserve(size);
	for(size_t i = 0; i < size - 1; ++i) mStrData.back()[i] = aStr[i];
	mStrData.back()[size - 1] = 0;
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
	add(aStr);
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
		moveStrToStr(std::move(aStr));
	}
	else
	{
		addStrToChar(aStr);
	}
}