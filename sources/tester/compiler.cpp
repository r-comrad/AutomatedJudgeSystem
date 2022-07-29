#include "compiler.hpp"

//--------------------------------------------------------------------------------

#include "domain/path.hpp"
#include "domain/error_message.hpp"

#include "main/path.hpp"

#include "process/pipe_windows_process.hpp"

//--------------------------------------------------------------------------------

dom::CharArrayTable
test::Compiler::getExecutableCommand(CodeInfo&& aCode) const noexcept
{
    dom::CharArrayTable result;

    auto fileLanguage = Language(mLanguages.tree.get(aCode.getFileLanguage()));

    switch (fileLanguage)
    {
    case Language::MAGIC_CPP:
        result = prepareCommandForCPP(aCode);
        break;
    
    case Language::SNAKE:
        result =  prepareCommandForPython(aCode);
        break;
    }

    return result;
}

//--------------------------------------------------------------------------------

test::Compiler::LanguageTree test::Compiler::mLanguages;

//--------------------------------------------------------------------------------

test::Compiler::LanguageTree::LanguageTree() 
{
    tree.add("cpp", uint32_t(Language::MAGIC_CPP));
    tree.add("py", uint32_t(Language::SNAKE));
}

//--------------------------------------------------------------------------------

dom::CharArrayTable
test::Compiler::prepareCommandForCPP(CPPInfo&& aInfo) const noexcept
{
    dom::CharArrayTable compileCommand;

    //TODO: Move
    compileCommand.emplace_back(CPP_COMPILER_NAME);
    compileCommand.emplace_back(std::move(aInfo.inputFileName));    
    compileCommand.emplace_back(std::move(aInfo.outputFileileName));
    compileCommand.back() += ".exe";

    //TODO: compiler log output
    proc::PipeWindowsProcess compiler;
    compiler.setComand(compileCommand);
    compiler.create();
    compiler.run();
    #ifdef _DBG_
    std::string s;
    compiler.read(s);
    WRITE_LOG("Compiler_output:", s);
    #endif

    dom::CharArrayTable result;
    result.emplace_back(std::move(compileCommand.back()));

    return result;
}

//--------------------------------------------------------------------------------

dom::CharArrayTable
test::Compiler::prepareCommandForPython(CPPInfo&& aInfo) const noexcept
{
    dom::CharArrayTable result;
    result.emplace_back("python3");
    result.emplace_back(std::move(aInfo.inputFileName));
    return result;
}

//--------------------------------------------------------------------------------
