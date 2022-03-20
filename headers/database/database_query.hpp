#ifndef DATABASE_QUERY_HPP
#define DATABASE_QUERY_HPP

//--------------------------------------------------------------------------------
//							DATABASE QUERYS HANDLER DECLARATION
//--------------------------------------------------------------------------------

#include <fstream>

#include "domain/domain.hpp"
#include "domain/paths.hpp"
#include "database/database.hpp"
#include "domain/my_strings.hpp"

#include "structs/problem_information.hpp"
#include "structs/testlib_message.hpp"

//--------------------------------------------------------------------------------

class MDatabaseQuery
{
public:
	/*
	\brief Construct querys handler for base with specifien name
	\param aDatabasePath Path to database.
	*/
	MDatabaseQuery(str_const_ref aDatabasePath);
	~MDatabaseQuery();

	/*
	\brief Retrieves the problem information (ID, file names, restrictions) 
	from the database and puts it in the ProblemInformation structure.
	\param aSudmissionInformation ProblemInformation structure for obtaining data.
	*/
	void prepareForTesting(ProblemInformation& aSudmissionInformation);

	/*
	\brief Writing result of submission check in database.
	\param aID Submission ID.
	\param aResult Result of submission check.
	\param aTime Submission time usage.
	\param aMemory Submission memory usage.
	*/
	void writeResult(int aID, str_const_ref aResult, int aTime, int aMemory);

	/*
	\brief Retrieves the next test from the database and puts it in 
	TestLibMessage struct. 
	(according to current statment step and 
	numbering of tests in the database)
	\param aSudmissionInformation Problem info (ID) for test search.
	\param TestLibMessage  TestLibMessage structure for obtaining test.
	*/
	void getNextTest(ProblemInformation& aSudmissionInformation, TestLibMessage& aTLM);

	/*
	\brief Extracts all problem tests from the database and puts it in files.
	\param aSudmissionInformation Problem info (ID) for tests search.
	*/
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

//--------------------------------------------------------------------------------

#endif // !DATABASE_QUERY_HPP