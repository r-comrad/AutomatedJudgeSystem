#ifndef COMPILER_HPP
#define COMPILER_HPP

//--------------------------------------------------------------------------------

#include <vector>

#include "domain/domain.hpp"
#include "domain/my_strings.hpp"
#include "domain/errors.hpp"
#include "domain/paths.hpp"

#include "process/pipe_process.hpp"

namespace sys {
	class Compiler
	{
	private:
		enum Language
		{
			NUN = 0,
			MAGIC_CPP = 1,
			SNAKE = 2
		};

	public:
		Compiler();
		virtual ~Compiler();

		void compile
		(
			std_string_ref aFileName, std_string_ref aOutName,
			Language aLanguage,
			std::vector<char*>& aCpmandParameters
		);
		void makeExecutable
		(
			std_string_ref aFileName, std_string_ref aOutputName,
			std::vector<char*>& aCpmandParameters
		);
	
	private:
		Compiler::Language getLanguage(std_string_ref aFileName);
	};
}

//--------------------------------------------------------------------------------

#endif // !COMPILER_HPP