#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

#include <string>

#include "database.h"
#include "submission_information.h"

class MDatabaseQuery
{
public:
	//MDatabaseQuery() {};
	MDatabaseQuery(std::wstring aDatabasePath);
	~MDatabaseQuery();
	
	//takeProblemId(sqlite3* ppDb);
	//coutRows(sqlite3* ppDb, int problemNum);
	//takeProblemId(sqlite3* ppDb, int problemNum);

	void makeTestCatalog(SubmissionInformation& aSudmissionInformation);
	void writeResult(int aID, std::string aResult, int aTime, int aMemory);

private:
	Database mDatabase;

	void getIDInformation(SubmissionInformation& aSudmissionInformation);
	void getLimitsInformation(SubmissionInformation& aSudmissionInformation);
	void getTests(int aContestID, SubmissionInformation& aSudmissionInformation);
};

#endif