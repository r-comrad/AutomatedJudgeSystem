#ifndef CODE_INFO_HPP
#define CODE_INFO_HPP

#include <vector>
#include <memory.h>
#include "main/path.hpp"

#include "domain/string.hpp"

namespace cor
{
    struct CPPInfo
    {
        dom::String inputFileName;
        dom::String outputFileileName;
        dom::String compilerName;
    };

    struct PythonInfo
    {
        dom::String fileName;
        dom::String interpretatorName;
    };

    class CodeInfo
    {
    public:
        enum class CodeInfoType { Submission, Checker };
        
        CodeInfo(CodeInfoType aType) noexcept;
        ~CodeInfo() noexcept = default;

        CodeInfo(const CodeInfo& other) = delete;
        CodeInfo& operator= (const CodeInfo& other) = delete;

        CodeInfo(CodeInfo&& other) noexcept = default;
        CodeInfo& operator= (CodeInfo&& other) noexcept = default;

        operator CPPInfo() noexcept;
        operator PythonInfo() noexcept;

        bool isLanguageStated() const noexcept;
        std::string getFileLanguage() const noexcept;

        void setFileName(dom::String&& aStr) noexcept;
        void setDesirableOutputFileName(dom::String&& aStr) noexcept;
        void setLanguage() noexcept;
        void setLanguage(dom::String&& aStr) noexcept;

    private:
        dom::StringTable mCodeData;
        std::optional <std::string> getFileExtension() const noexcept;

        // First    cell            - submission file name
        // Secind   cell [optional] - output file name
        // Therd    cell [optional] - language
        // ...
        enum class CellMap {Submission = 0, Output = 1, Language = 2};
    };
}

#endif // !CODE_INFO_HPP