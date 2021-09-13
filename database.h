#ifndef DATABASE_H
#define DATABASE_H

#include <string>

#include <iostream>
#include "sqlite3.h"
#include <string>

#include "Domain.h"

typedef sqlite3 Base;
typedef sqlite3_stmt Statement;

class Database
{
public:
	Database(std::string aPath);
	void select(std::string aTableName, std::string aColum = "", std::string aConditon = "");
	auto getNextRow(int aColumNumber);
	void clouseStatment();

private:
	Base* mBase;
	Statement* mStatement;
};

#endif