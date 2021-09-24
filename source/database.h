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
	void clouseStatment();
	//int takeProblemId(sqlite3* ppDb);

	void step();

	const unsigned char* getTextFromRow(int aColumNumber);
	int getIntFromRow(int aColumNumber);
	sint_64 getInt64FromRow(int aColumNumber);

private:
	Base* mBase;
	Statement* mStatement;
};

#endif