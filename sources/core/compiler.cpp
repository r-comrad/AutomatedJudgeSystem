#include "core/compiler.hpp"

//--------------------------------------------------------------------------------
//					COMPILER AND INTERPRETATOR IMPLEMENTATION
//--------------------------------------------------------------------------------

sys::Compiler::Compiler() 
{
	mLanguageTypes.add("cpp",uint_32(Language::MAGIC_CPP));
    mLanguageTypes.add("py",uint_32(Language::SNAKE));
}

//--------------------------------------------------------------------------------

// void
// sys::Compiler::prepareExecutableCommand
// (
//     str_const_ref aFileName,
//     str_const_ref aOutName,
//     std::vector<char*>& aCpmandParameters
// )
// {

//     sys::Compiler::Language language = getLanguage(aFileName);

// 	if (language == Language::MAGIC_CPP)
//     {
//         aCpmandParameters.push_back(newStrCopy((aOutName + ".exe")));
//         std::vector<char*> comand;

// #ifdef BILL_WINDOWS 
//         comand.push_back(newStrCopy((CPP_COMPILER_NAME)));
// #elif defined(LINUS_LINUX)
//         comand.push_back(newCharPtrCopy("g++"));
// #endif

//         comand.push_back(newStrCopy(aFileName));

// #ifdef LINUS_LINUX
//         comand.push_back(newCharPtrCopy("-o"));
// #endif

//         comand.push_back(aCpmandParameters.back());
//         comand.push_back(NULL);
//         PipeProcess compiler(comand);
//         compiler.run();
//         delete comand[0];
//         delete comand[1];

// #if defined(_DBG_) && defined(COMPILER_LOG_OUTPUT)
//         str_val compilerOutput;
//         compiler.readPipe(compilerOutput);
//         WRITE_LOG(compilerOutput);
// #endif
//     }
//     else if (language == Language::SNAKE)
//     {
// #ifdef BILL_WINDOWS
//         aCpmandParameters.push_back(newCharPtrCopy(""));
// #endif
//         aCpmandParameters.push_back(newCharPtrCopy("python3"));
//         aCpmandParameters.push_back(newStrCopy(aFileName));
//     }
//     else
//     {
//         WRITE_ERROR("Compiler", "prepareExecutableCommand", 10, "undefined_language");
//     }
//     aCpmandParameters.push_back(NULL);
// }

StringTable
sys::Compiler::getExecutableCommand(cor::CodeInfo&& aCode) const noexcept
{
	std::vector<std::unique_ptr<char[]>> result;

	sys::Compiler::Language fileLanguage;
	if (aCode.isLanguageStated())
	{
        //TODO:
		// fileLanguage = aCode.getFileLanguage();
		// fileLanguage = getLanguage(aCode.getFileExtension());
	}
	else
	{
		fileLanguage = getLanguage(aCode.getFileExtension());
	}

	if (fileLanguage == Language::MAGIC_CPP)
    {
		result = prepareCommandForCPP(aCode);
	}
	else if (fileLanguage == Language::SNAKE)
    {
		result = prepareCommandForPython(aCode);
	}

	return result;
}

StringTable
sys::Compiler::prepareCommandForCPP(cor::CPPInfo&& aInfo) const noexcept
{
	std::vector<std::unique_ptr<char[]>> result(1);
	std::vector<std::unique_ptr<char[]>> compileCommand(3);

	std::string exeFile(aInfo.outputFileileName.get());
	exeFile.append(".exe");
	result[0].reset(newStrCopy(exeFile));

	auto compName = CPP_COMPILER_NAME;
	compileCommand[0].reset(newStrCopy(compName));
	compileCommand[1] = std::move(aInfo.inputFileName);
	compileCommand[2].reset(newStrCopy(exeFile));

	proc::PipeProcess process(compileCommand);
	process.run();

	return result;
}

StringTable
sys::Compiler::prepareCommandForPython(cor::CPPInfo&& aInfo) const noexcept
{
	std::vector<std::unique_ptr<char[]>> result(2);
	result[0].reset(newCharPtrCopy("python3"));
	result[1] = std::move(aInfo.inputFileName);
	return result;
}

//--------------------------------------------------------------------------------

sys::Compiler::Language
sys::Compiler::getLanguage(const std::string& aFileName) const noexcept
{
	sys::Compiler::Language result = sys::Compiler::Language::NUN;
	if (!aFileName.empty())
	{
		result = Language(mLanguageTypes.get(aFileName));
	}
	return result;
}

//--------------------------------------------------------------------------------