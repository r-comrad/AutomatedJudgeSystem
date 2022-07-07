#ifndef CODE_INFO_HPP
#define CODE_INFO_HPP

#include <vector>
#include <memory.h>

#include "domain/string.hpp"

namespace cor
{
	struct CPPInfo
	{
		CharArray compilerName;
		CharArray inputFileName;
		CharArray outputFileileName;
	};

	struct PythonInfo
	{
		CharArray interpretatorName;
		CharArray fileName;
	};

	class CodeInfo
	{
	public:
		operator CPPInfo() noexcept;
		operator PythonInfo() noexcept;

		bool isLanguageStated() const noexcept;
		std::string getFileLanguage() const noexcept;
		std::string getFileExtension() const noexcept;

		CodeInfo(CharArray&& fileName, CharArray&& outputFileileName) noexcept;

	private:
		std::vector<std::unique_ptr<char[]>> mCodeData;
		// First	cell			- language
		// Secind	cell			- input file name
		// Therd	cell [optional]	- output file name
	};
}

#endif // !CODE_INFO_HPP