#include "code_info.hpp"

cor::CodeInfo::CodeInfo(CodeInfoType aType) noexcept :
    mCodeData(3)
{
    mCodeData[size_t(CellMap::Submission)] = MAEDIA;
    if (aType == CodeInfoType::Submission)
    {
       mCodeData[size_t(CellMap::Output)] = SOLUTION_PATH + "sol-";
    }
    else
    {
       mCodeData[size_t(CellMap::Output)] = CHECKER_PATH + "ch-";
    }
}

cor::CodeInfo::operator cor::CPPInfo() noexcept
{
    return CPPInfo 
    { 
        std::move(mCodeData[size_t(CellMap::Submission)]), 
        std::move(mCodeData[size_t(CellMap::Output)]),
        std::move(mCodeData[size_t(CellMap::Language)])
    };
}

cor::CodeInfo::operator cor::PythonInfo() noexcept
{
    return PythonInfo
    { 
        std::move(mCodeData[size_t(CellMap::Submission)]), 
        std::move(mCodeData[size_t(CellMap::Language)])
    };
}

bool
cor::CodeInfo::isLanguageStated() const noexcept
{
    return !mCodeData[size_t(CellMap::Language)].isEmpty();
}

std::string
cor::CodeInfo::getFileLanguage() const noexcept
{
    return std::string(mCodeData[size_t(CellMap::Language)]);
}

void
cor::CodeInfo::setFileName(dom::String&& aStr) noexcept
{
    mCodeData[size_t(CellMap::Submission)] += std::move(aStr);
    mCodeData[size_t(CellMap::Submission)].merge();
}

void
cor::CodeInfo::setDesirableOutputFileName(dom::String&& aStr) noexcept
{
    mCodeData[size_t(CellMap::Output)] += std::move(aStr);
    mCodeData[size_t(CellMap::Output)].merge();
}

void
cor::CodeInfo::setLanguage() noexcept
{
    //TODO: check
    mCodeData[size_t(CellMap::Language)] = getFileExtension().value();
    mCodeData[size_t(CellMap::Language)].merge();
}

void
cor::CodeInfo::setLanguage(dom::String&& aStr) noexcept
{
    mCodeData[size_t(CellMap::Language)] = std::move(aStr);
    mCodeData[size_t(CellMap::Language)].merge();
}

std::optional <std::string>
cor::CodeInfo::getFileExtension() const noexcept
{
   return mCodeData[size_t(CellMap::Submission)].backSubStr('.');
}