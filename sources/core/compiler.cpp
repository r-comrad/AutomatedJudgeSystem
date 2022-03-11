#include "core/compiler.hpp"

//--------------------------------------------------------------------------------

sys::Compiler::Compiler() {}

//--------------------------------------------------------------------------------

sys::Compiler::~Compiler() {}

//--------------------------------------------------------------------------------

void
sys::Compiler::compile
(
    std_string_ref aFileName,
    std_string_ref aOutName,
    Language aLanguage,
    std::vector<char*>& aCpmandParameters
)
{
    if (aLanguage == Language::MAGIC_CPP)
    {
        aCpmandParameters.push_back(newStrCopy((aOutName + ".exe")));
        std::vector<char*> comand;

#ifdef BILL_WINDOWS
        comand.push_back(newStrCopy((CPP_COMPILER_NAME)));
#elif defined(LINUS_LINUX)
        comand.push_back(newCharPtrCopy("g++"));
#endif

        comand.push_back(newStrCopy(aFileName));

#ifdef LINUS_LINUX
        comand.push_back(newCharPtrCopy("-o"));
#endif

        comand.push_back(aCpmandParameters.back());
        comand.push_back(NULL);
        PipeProcess compiler(comand);
        compiler.run();
        delete comand[0];
        delete comand[1];

#if defined(_DBG_) && defined(COMPILER_LOG_OUTPUT)
        std_string compilerOutput;
        compiler.readPipe(compilerOutput);
        WD_LOG(compilerOutput);
#endif
    }
    else if (aLanguage == Language::SNAKE)
    {
#ifdef BILL_WINDOWS
        aCpmandParameters.push_back(newCharPtrCopy(""));
#endif
        aCpmandParameters.push_back(newCharPtrCopy("python3"));
        aCpmandParameters.push_back(newStrCopy(aFileName));
    }
    aCpmandParameters.push_back(NULL);
}

//--------------------------------------------------------------------------------

void
sys::Compiler::makeExecutable
(
    std_string_ref aFileName,
    std_string_ref aOutputName,
    std::vector<char*>& aCpmandParameters
)
{
    sys::Compiler::Language language = getLanguage(aFileName);
    return compile(aFileName, aOutputName, language, aCpmandParameters);
}

//--------------------------------------------------------------------------------

sys::Compiler::Language
sys::Compiler::getLanguage(std_string_ref aFileName)
{
    int num = aFileName.find('.');
    if (num == -1) return sys::Compiler::Language::NUN;

    sys::Compiler::Language result = sys::Compiler::Language::NUN;
    std::string fileExtension = aFileName.substr(num + 1);
    if (fileExtension == "cpp") result = sys::Compiler::Language::MAGIC_CPP;
    if (fileExtension == "py") result = sys::Compiler::Language::SNAKE;
    return result;
}

//--------------------------------------------------------------------------------