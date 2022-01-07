#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

#include <string>

#include "database.h"
#include "submission_information.h"
#include "testlib_message.h"

class MDatabaseQuery
{
public:
	MDatabaseQuery(std::string aDatabasePath);
	~MDatabaseQuery();

	void prepareForTesting(SubmissionInformation& aSudmissionInformation);
	void writeResult(int aID, std::string aResult, int aTime, int aMemory);

	void getNextTest(SubmissionInformation& aSudmissionInformation, TestLibMessage& aTLM);
	void getAllTests(SubmissionInformation& aSudmissionInformation);
private:
	Database mDatabase;
	int mReservedStatementNumber;
public:
	void prepareTestsStatement(SubmissionInformation& aSudmissionInformation);
private:
	void getIDInformation(SubmissionInformation& aSudmissionInformation);
	void getLimitsInformation(SubmissionInformation& aSudmissionInformation);
};

#endif