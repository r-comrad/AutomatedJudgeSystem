#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

#include <string>

#include "database.h"

class MDatabaseQuery
{
public:
	//MDatabaseQuery() {};
	MDatabaseQuery(std::wstring aDatabasePath, std::wstring aTaskPath);
	~MDatabaseQuery();
	
	//takeProblemId(sqlite3* ppDb);
	//coutRows(sqlite3* ppDb, int problemNum);
	//takeProblemId(sqlite3* ppDb, int problemNum);

	void makeTestCatalog(int aID);
	void writeResult(std::string aResult, int aTime, int aMemory);

private:
	Database mDatabase;

	void getIDInformation(int aID);
	void getLimitsInformation();
	void getTests();

	std::ofstream mTaskFile;

	std::wstring mTaskPath;

	int mGlobalId;
	int mContestID;
	const unsigned char* mFileName;

	int mTimeLimit;
	int mMemoryLimit;
};

#endif