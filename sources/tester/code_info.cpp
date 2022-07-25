#include "code_info.hpp"

//--------------------------------------------------------------------------------

#include "main/path.hpp"

//--------------------------------------------------------------------------------

test::CodeInfo::CodeInfo(CodeInfoType aType) noexcept :
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

//--------------------------------------------------------------------------------

test::CodeInfo::operator test::CPPInfo() noexcept
{
    return CPPInfo 
    { 
        std::move(mCodeData[size_t(CellMap::Submission)]), 
        std::move(mCodeData[size_t(CellMap::Output)]),
        std::move(mCodeData[size_t(CellMap::Language)])
    };
}

//--------------------------------------------------------------------------------

test::CodeInfo::operator test::PythonInfo() noexcept
{
    return PythonInfo
    { 
        std::move(mCodeData[size_t(CellMap::Submission)]), 
        std::move(mCodeData[size_t(CellMap::Language)])
    };
}

//--------------------------------------------------------------------------------

std::string
test::CodeInfo::getFileLanguage() const noexcept
{
    return std::string(mCodeData[size_t(CellMap::Language)]);
}

//--------------------------------------------------------------------------------

void
test::CodeInfo::setFileName(dom::String&& aStr) noexcept
{
    mCodeData[size_t(CellMap::Submission)] += std::move(aStr);
    mCodeData[size_t(CellMap::Submission)].merge();
}

//--------------------------------------------------------------------------------

void
test::CodeInfo::setDesirableOutputFileName(dom::String&& aStr) noexcept
{
    mCodeData[size_t(CellMap::Output)] += std::move(aStr);
    mCodeData[size_t(CellMap::Output)].merge();
}

//--------------------------------------------------------------------------------

void
test::CodeInfo::setLanguage(dom::String&& aStr) noexcept
{
    if (aStr.isEmpty())
    {
        auto ext = getFileExtension();
        START_LOG_BLOCK ("GET_FILE_EXTENSION");
        WRITE_LOG       ("HAS_FILE_EXTENSION:", ext.has_value());
        END_LOG_BLOCK   ("FILE_EXTENSION:",     ext.value());
        mCodeData[size_t(CellMap::Language)] = getFileExtension().value();
    }
    else
    {
        mCodeData[size_t(CellMap::Language)] = std::move(aStr);
    }
    mCodeData[size_t(CellMap::Language)].merge();
}

//--------------------------------------------------------------------------------

std::optional <std::string>
test::CodeInfo::getFileExtension() const noexcept
{
   return mCodeData[size_t(CellMap::Submission)].backSubStr('.');
}

//--------------------------------------------------------------------------------
