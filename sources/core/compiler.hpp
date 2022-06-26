#ifndef COMPILER_HPP
#define COMPILER_HPP

//--------------------------------------------------------------------------------
//						COMPILER AND INTERPRETER DECLARATION
//--------------------------------------------------------------------------------

#define _CRT_SECURE_NO_WARNINGS

#include <vector>

#include "main/flags.hpp"

#include "domain/type.hpp"
#include "domain/my_string.hpp"
#include "domain/error_message.hpp"
#include "domain/path.hpp"
#include "process/pipe_process.hpp"

#include "algorithms/suffix_tree.hpp"

//--------------------------------------------------------------------------------

namespace sys {
	class Compiler
	{
	private:
		enum class Language
		{
			NUN = 0,
			MAGIC_CPP = 1,
			SNAKE = 2
		};

	public:
		Compiler();
		//virtual ~Compiler();
		virtual ~Compiler() = default;

		/*
		\brief Prepare a command that executes the child process and
		compiles the file in specified directory if necessary.
		\param aFileName A file with the code to execute.
		\param aOutputName Specified compiler output path.
		\param aCpmandParameters An array containing the resulting command.
		*/
		void prepareExecutableCommand
		(
			str_const_ref aFileName,
			str_const_ref aOutName,
			std::vector<char*>& aCpmandParameters
		);
	
	private:
		/*
		\brief Determining the language of the file based on 
		the file extension.
		\param aFileName A file with the code path.
		\return Returns the language of the passed file.
		*/
		Compiler::Language getLanguage(str_const_ref aFileName);

		alg::SuffixTree mLanguageTypes;
	};
}

//--------------------------------------------------------------------------------

#endif // !COMPILER_HPP


//		/*
//		\brief Prepare a command that executes the child process and
//		compiles the file in specified directory if necessary.
//		\param aFileName A file with the code to execute path.
//		\param aOutputName Specified compiler output path.
//		\param aCpmandParameters An array containing the resulting command.
//		*/
//void prepareExecutableCommand
//(
//	str_const_ref aFileName,
//	str_const_ref aOutputName,
//	std::vector<char*>& aCpmandParameters
//);