#ifndef DATABASE_QUERY_H
#define DATABASE_QUERY_H

#include <string>
#include <fstream>

#include "domain/domain.hpp"
#include "domain/paths.hpp"
#include "database/database.hpp"
#include "structs/problem_information.hpp"
#include "structs/testlib_message.hpp"

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