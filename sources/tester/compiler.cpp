#include "compiler.hpp"

//--------------------------------------------------------------------------------

#include "domain/type.hpp"
#include "domain/path.hpp"
#include "domain/error_message.hpp"

#include "main/path.hpp"

#include "process/pipe_windows_process.hpp"

//--------------------------------------------------------------------------------

dom::StringTable
test::Compiler::getExecutableCommand(CodeInfo&& aCode) const noexcept
{
    dom::StringTable result;

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
    tree.add("cpp", uint_32(Language::MAGIC_CPP));
    tree.add("py", uint_32(Language::SNAKE));
}

//--------------------------------------------------------------------------------

dom::StringTable
test::Compiler::prepareCommandForCPP(CPPInfo&& aInfo) const noexcept
{
    dom::StringTable compileCommand;

    //TODO: Move
    compileCommand.emplace_back(CPP_COMPILER_NAME);
    compileCommand.emplace_back(std::move(aInfo.inputFileName));    
    compileCommand.emplace_back(std::move(aInfo.outputFileileName));
    compileCommand.back() += ".exe";
    compileCommand.back().merge();

    //TODO: compiler log output
    proc::PipeWindowsProcess compiler;
    compiler.setComand(compileCommand);
    compiler.create();
    compiler.run();

    dom::StringTable result;
    result.emplace_back(std::move(compileCommand.back()));

    return result;
}

//--------------------------------------------------------------------------------

dom::StringTable
test::Compiler::prepareCommandForPython(CPPInfo&& aInfo) const noexcept
{
    dom::StringTable result;
    result.emplace_back("python3");
    result.emplace_back(std::move(aInfo.inputFileName));
    return result;
}

//--------------------------------------------------------------------------------
