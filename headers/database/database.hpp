#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <iostream>
#include <vector>

#include "sqllite/sqlite3.h"

#include "domain/domain.hpp"
#include "domain/my_strings.hpp"
#include "domain/errors.hpp"
#include "domain/pair.hpp"

typedef sqlite3 Base;
typedef sqlite3_stmt Statement;

class Database
{
public:
	Database(std_string aPath);
	void select(std_string aTableName, std_string aColum = "", std_string aConditon = "", int aStatementID = 0);
	void update(std_string aTableName, std_string aValue, std_string aConditon, int aStatementID = 0);
	void closeStatment(int aStatementID = 0);
	int	step(int aStatementID = 0);

	const unsigned char* getTextFromRow(int aColumNumber, int aStatementID = 0);
	const void* getText16FromRow(int aColumNumber, int aStatementID = 0);
	int getIntFromRow(int aColumNumber, int aStatementID = 0);
	sint_64 getInt64FromRow(int aColumNumber, int aStatementID = 0);
	char* toAscii(const unsigned char* input, int aStatementID = 0);

private:
	Base* mBase;
	std::vector<Statement*> mStatement;

	void prepare(std_string_ref aStatment, int aStatementID);
};

#endif