#include "code_info.hpp"

cor::CodeInfo::CodeInfo(CharArray&& fileName, CharArray&&outputFileileName) noexcept :
	mCodeData(3, nullptr)
{
	mCodeData[1] = std::move(fileName);
	mCodeData[2] = std::move(outputFileileName);
}

cor::CodeInfo::operator cor::CPPInfo() noexcept
{
	return CPPInfo 
	{ 
		std::move(mCodeData[0]), 
		std::move(mCodeData[1]),
		std::move(mCodeData[2])
	};
}

cor::CodeInfo::operator cor::PythonInfo() noexcept
{
	return PythonInfo
	{ 
		std::move(mCodeData[0]), 
		std::move(mCodeData[1])
	};
}

bool
cor::CodeInfo::isLanguageStated() const noexcept
{
	return mCodeData[0].get() == nullptr;
}

std::string
cor::CodeInfo::getFileLanguage() const noexcept
{
	return std::string(mCodeData[0].get());
}

std::string
cor::CodeInfo::getFileExtension() const noexcept
{
	std::string result = "";

	int strSize = strlen(mCodeData[1].get());
	int num = -1;
	for(int i = strSize; i >= 0; --i)
	{
		if ( mCodeData[1][i] == '.') 
		{
			num = i;
			break;
		}
	}

	if (num > -1)
	{
		for(int i = strSize; i >= strSize - num; --i)
		{
			result.push_back(mCodeData[1][i]);
		}
	}

	return result;
}
