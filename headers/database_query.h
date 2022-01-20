#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

#include <string>

#include "database.h"
#include "problem_information.h"
#include "testlib_message.h"

class MDatabaseQuery
{
public:
	MDatabaseQuery(std::string aDatabasePath);
	~MDatabaseQuery();

	void prepareForTesting(ProblemInformation& aSudmissionInformation);
	void writeResult(int aID, std::string aResult, int aTime, int aMemory);

	void getNextTest(ProblemInformation& aSudmissionInformation, TestLibMessage& aTLM);
	void getAllTests(ProblemInformation& aSudmissionInformation);
private:
	Database mDatabase;
	int mReservedStatementNumber;
public:
	void prepareTestsStatement(ProblemInformation& aSudmissionInformation);
private:
	void getIDInformation(ProblemInformation& aSudmissionInformation);
	void getLimitsInformation(ProblemInformation& aSudmissionInformation);
};

#endif