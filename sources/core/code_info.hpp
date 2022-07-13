#ifndef CODE_INFO_HPP
#define CODE_INFO_HPP

#include <vector>
#include <memory.h>

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
        operator CPPInfo() noexcept;
        operator PythonInfo() noexcept;

        bool isLanguageStated() const noexcept;
        std::string getFileLanguage() const noexcept;
        std::string getFileExtension() const noexcept;

        CodeInfo(dom::String&& fileName, dom::String&& outputFileileName) noexcept;

    private:
        dom::StringTable mCodeData;
        // First    cell            -  input file name
        // Secind   cell [optional] - output file name
        // Therd    cell [optional] - language
        // ...
    };
}

#endif // !CODE_INFO_HPP