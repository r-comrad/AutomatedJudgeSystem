#include "core/compiler.hpp"

//--------------------------------------------------------------------------------
//					COMPILER AND INTERPRETATOR IMPLEMENTATION
//--------------------------------------------------------------------------------

sys::Compiler::Compiler() {}

//--------------------------------------------------------------------------------

sys::Compiler::~Compiler() {}

//--------------------------------------------------------------------------------

void
sys::Compiler::prepareExecutableCommand
(
    str_const_ref aFileName,
    str_const_ref aOutName,
    std::vector<char*>& aCpmandParameters
)
{
    sys::Compiler::Language language = getLanguage(aFileName);

    if (language == Language::MAGIC_CPP)
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
        str_val compilerOutput;
        compiler.readPipe(compilerOutput);
        WD_LOG(compilerOutput);
#endif
    }
    else if (language == Language::SNAKE)
    {
#ifdef BILL_WINDOWS
        aCpmandParameters.push_back(newCharPtrCopy(""));
#endif
        aCpmandParameters.push_back(newCharPtrCopy("python3"));
        aCpmandParameters.push_back(newStrCopy(aFileName));
    }
    else
    {
        WD_ERROR(compiler.0, "undefined language");
    }
    aCpmandParameters.push_back(NULL);
}

//--------------------------------------------------------------------------------

sys::Compiler::Language
sys::Compiler::getLanguage(str_const_ref aFileName)
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