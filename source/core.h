#ifndef CORE_H
#define CORE_H

#include "database_query.h"

class Core
{
public:
	Core(int aId);


private:
	enum Language { NUN = 0, MagicCPP, Snake};

	Base* mBase;
	Statement* mStatement;

	
	void compile(std::wstring aCheckerName, std::wstring aOutName, Language aLanguage);
	void makeChecker(std::wstring aCheckerNam);
	Core::Language getLanguage(std::wstring aFileName);

};

#endif //CORE_H