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

// std::string
// getString
// (
//     const unsigned char* aString
// ) noexcept
// {
//     str_val result;
//     for (int i = 0; aString[i]; ++i) result.push_back(aString[i]);
//     return result;
// }

// std::string 
// getString(const StringTable& aStrArray) noexcept
// {
// 	std::string result;

// 	int size = 0;
// 	for(auto& str : aStrArray) size += strlen(str.get());
// 	result.reserve(size + 1);

// 	for(auto& str : aStrArray) 
// 	{
// 		result.append(str.get());
// 	}

// 	return result;
// }

// std::unique_ptr<char[]>
// getCharArray(const StringTable& aStrArray) noexcept
// {
// 	std::unique_ptr<char[]> result;

// 	int size = 0;
// 	for(auto& str : aStrArray) size += strlen(str.get());
// 	result.reset(new char[size + 1]);

// 	for(auto& str : aStrArray) 
// 	{
// 		strcat(result.get(), str.get());
// 	}

// 	return result;
// }

// CharArray
// getCharArray
// (
//     const unsigned char* aString
// ) noexcept
// {
//     int size = 0;
// 	while (aString[size++]);
// 	CharArray result {new char[size]};
//     for (int i = 0;i < size; ++i) 
// 	{
// 		result[i] = aString[i];
// 	}
//     return result;
// }

// #ifdef BILL_WINDOWS
// void
// makeWindowString
// (
//     str_ref aString
// )
// {
//     for (char& i : aString)
//     {
//         if (i == '/') i = '\\';
//     }
// }

// #endif // !BILL_WINDOWS

//------------------------------------------------------

dom::String::String() noexcept :
	mType(StrType::NonDetermin)
{}

dom::String::String(const char* aStr) noexcept :
	mType	(StrType::CharArray)
{
	add(aStr);
}

dom::String::String(const unsigned char* aStr) noexcept :
	String	((const char*)aStr)
{}

dom::String::String(const std::string& aStr) noexcept :
	mType	(StrType::String)
{
	add(aStr);
}

dom::String::String(std::string&& aStr) noexcept :
	mType	(StrType::String)
{
	add(std::move(aStr));
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

dom::String::operator CharArray() const noexcept
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

CharArray
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

//------------------------------------------------------

auto
dom::String::getSizeLumbda(const char* str) const noexcept
{
	return [](const char* str) noexcept { return strlen(str); };
}

auto
dom::String::getSizeLumbda(const CharArray& str) const noexcept
{
	return [](const CharArray& str) noexcept { return strlen(str.get()); };
}

auto
dom::String::getSizeLumbda(const std::string& str) const noexcept
{
	return [](const std::string& str) noexcept { return str.size(); };
}

//auto
//dom::String::getEmplaceLumbda(const CharArray& str) const noexcept
//{
//	return [](const CharArray& str, int num, ) noexcept { return strlen(str.get()); };
//}
//
//auto
//dom::String::getEmplaceLumbda(const std::string& str) const noexcept
//{
//	return [](const std::string& str, ) noexcept { return str.size(); };
//}

auto
dom::String::getConstructorLumbda(const char* str) const noexcept
{
	return [](size_t aSize) noexcept { return new char[aSize + 1]; };
}

auto
dom::String::getConstructorLumbda(const CharArray& str) const noexcept
{
	return [](size_t aSize) noexcept { return std::move(std::make_unique<char[]>(aSize + 1)); };
}

auto 
dom::String::getConstructorLumbda(const std::string& str) const noexcept
{
	return [](size_t aSize) noexcept
	{
		std::string result;
		result.resize(aSize);
		return result;
	};
}

void
dom::String::clear(StrType aType) noexcept
{
	mType = aType;
	mCharData.resize(0);
	mStrData.resize(0);
}