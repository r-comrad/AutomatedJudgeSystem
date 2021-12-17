#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <iostream>
#include <vector>

#include "sqlite3.h"

#include "domain.h"

typedef sqlite3 Base;
typedef sqlite3_stmt Statement;

class Database
{
public:
	Database(std::wstring aPath);
	void select(std::string aTableName, std::string aColum = "", std::string aConditon = "", int aStatementID = 0);
	void update(std::string aTableName, std::string aValue, std::string aConditon, int aStatementID = 0);
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

	void prepare(std::string& aStatment, int aStatementID);
};

#endif