#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

#include <string>

#include "database.h"

class DatabaseQuery
{
public:
	DatabaseQuery(std::wstring aDatabasePath, std::wstring aTaskPath);
	~DatabaseQuery();
	
	//takeProblemId(sqlite3* ppDb);
	//coutRows(sqlite3* ppDb, int problemNum);
	//takeProblemId(sqlite3* ppDb, int problemNum);

	void makeTestCatalog(int aID);

private:
	Database mDatabase;

	void getIDInformation(int aID);
	void getLimitsInformation();
	void getTests();

	std::ofstream mTaskFile;

	std::wstring mTaskPath;

	int mContestID;
	const unsigned char* mFileName;

	int mTimeLimit;
	int mMemoryLimit;
};

#endif