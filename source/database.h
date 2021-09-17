#ifndef DATABASE_H
#define DATABASE_H

#include <string>

#include "sqlite3.h"

#include "domain.h"

typedef sqlite3 Base;
typedef sqlite3_stmt Statement;

class Database
{
public:
	Database(std::string aPath);
	void select(std::string aTableName, std::string aColum = "", std::string aConditon = "");
	auto getNextRow(int aColumNumber);
	void clouseStatment();
	int takeProblemId(sqlite3* ppDb);

private:
	Base* mBase;
	Statement* mStatement;
};

#endif