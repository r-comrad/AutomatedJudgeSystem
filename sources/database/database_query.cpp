#include "database/database_query.hpp"

//--------------------------------------------------------------------------------
//					    DATABASE QUERYS HANDLER IMPLEMENTATION
//--------------------------------------------------------------------------------

//UPDATE core_solutions SET result = hh WHERE id = 10
//SELECT * FROM core_solutions WHERE id = 10
//UPDATE core_solutions SET lang = "C:\task1\checker\checker.cpp" WHERE id = 10
//UPDATE core_solutions SET file_name = "2/1.py" WHERE id = 3
//UPDATE core_contests SET time_limit = 1000 WHERE id = 20
//UPDATE core_contests SET memory_limit = 262144 WHERE id = 21

MDatabaseQuery::MDatabaseQuery
(
    const std::string& aDatabasePath
) :
    mDatabase                   (aDatabasePath),
    mReservedStatementNumber    (0)
{}

MDatabaseQuery::~MDatabaseQuery()
{}

ProblemInformation 
MDatabaseQuery::getProblemInfo(int ID)
{
	ProblemInformation result;
	result.mID = ID;

    getGeneralInformation(result);
    getLimitsInformation(result);

	return result;
}

void 
MDatabaseQuery::writeResult
(
    int                 aID, 
    const std::string&  aResult,
    int                 aTime, 
    int                 aMemory
)
{
    START_LOG_BLOCK("Updating_database");
    WRITE_LOG("Updating...");

    mDatabase.update("core_solutions", "result = '" + aResult + "'" +
        ", time = " + std::to_string(aTime) +
        ", memory = " + std::to_string(aMemory), "id = " + std::to_string(aID));
    mDatabase.step();
    mDatabase.closeStatment();

    END_LOG_BLOCK("Database_updated?");
}

void
MDatabaseQuery::getNextTest(ProblemInformation& aSudmissionInformation, TestLibMessage& aTLM)
{

    WRITE_LOG("Taking_next_test");
    mDatabase.step(mReservedStatementNumber);
    //if (mDatabase.step() != SQLITE_OK) break; TODO: fixing that shit
    const unsigned char* input = mDatabase.getTextFromRow(0, mReservedStatementNumber);
    const unsigned char* output = mDatabase.getTextFromRow(1, mReservedStatementNumber);
    if (input == NULL)
    {
        aSudmissionInformation.mTestsAreOver = true;
        aTLM.mTest = "";
        aTLM.mAnswer = "";
        return;
    }

    aSudmissionInformation.mTestsCount++;
    aTLM.mTest = std::string(reinterpret_cast<const char*>(input));
    //aTLM.mTest += "\n";
    aTLM.mAnswer = std::string(reinterpret_cast<const char*>(output));
}

void
MDatabaseQuery::getAllTests
(
    ProblemInformation& aSudmissionInformation
)
{
    START_LOG_BLOCK("Geting_test_from_database");

    mDatabase.select("core_test", "input, output", "contest_id = " + std::to_string(aSudmissionInformation.mContestID));
    int cnt = 0;
    for (;; ++cnt)
    {
        mDatabase.step();
        //if (mDatabase.step() != SQLITE_OK) break; TODO: fixing that shit
        const unsigned char* input = mDatabase.getTextFromRow(0);
        const unsigned char* output = mDatabase.getTextFromRow(1);
        if (input == NULL) break;

        std::ofstream taskFile(TEST_PATH + std::to_string(aSudmissionInformation.mID) + "-" + std::to_string(cnt));
        std::ofstream ansFile(ANSWERS_PATH + std::to_string(aSudmissionInformation.mID) + "-" + std::to_string(cnt));

        if (!taskFile.is_open())
        {
            WRITE_ERROR("MDatabaseQuery", 10, "Can't_open_file", TEST_PATH + std::to_string(cnt));
            continue;//TODO
        }
        if (!ansFile.is_open())
        {
            WRITE_ERROR("MDatabaseQuery", 11, "Can't_open_file", ANSWERS_PATH + std::to_string(cnt));
            continue;
        }

        for (int i = 0; input[i];) taskFile << input[i++];
        for (int i = 0; output[i];) ansFile << output[i++];
    }
    aSudmissionInformation.mTestsCount = cnt;

    mDatabase.closeStatment();

    END_LOG_BLOCK("Tests_taken_from_pashae");
}

void
MDatabaseQuery::prepareTestsStatement
(
    ProblemInformation& aSudmissionInformation
)
{
    aSudmissionInformation.mTestsAreOver = false;
    aSudmissionInformation.mTestsCount = 0;

    START_LOG_BLOCK("Prepare_geting_test_from_database");
    mDatabase.select("core_test", "input, output", "contest_id = " +
        std::to_string(aSudmissionInformation.mContestID), mReservedStatementNumber);
    END_LOG_BLOCK("I'm_ready");
}

void 
MDatabaseQuery::getGeneralInformation
(
    ProblemInformation& aSudmissionInformation
)
{
    START_LOG_BLOCK("Geting_ID_and_name_from_database");

    mDatabase.select("core_solutions", "file_name, contest_id", "id = " + std::to_string(aSudmissionInformation.mID));
    mDatabase.step();

    aSudmissionInformation.mSolutionFileName = getCharArray(mDatabase.getTextFromRow(0));
    aSudmissionInformation.mContestID = mDatabase.getInt64FromRow(1);
    mDatabase.closeStatment();

    WRITE_LOG("File_name:", aSudmissionInformation.mSolutionFileName.get());
    END_LOG_BLOCK("Contest_ID:", aSudmissionInformation.mContestID);
}

void 
MDatabaseQuery::getLimitsInformation
(
    ProblemInformation& aSudmissionInformation
)
{
    START_LOG_BLOCK("Geting_limits_from_database");

    mDatabase.select("core_contests", "time_limit, memory_limit, checker", "id = " + std::to_string(aSudmissionInformation.mContestID));
    mDatabase.step();
    aSudmissionInformation.mTimeLimit = mDatabase.getInt64FromRow(0);
    aSudmissionInformation.mMemoryLimit = mDatabase.getInt64FromRow(1);
    aSudmissionInformation.mCheckerFileName = getCharArray(mDatabase.getTextFromRow(2));
    mDatabase.closeStatment();

    WRITE_LOG("Time_limit:", aSudmissionInformation.mTimeLimit);
    END_LOG_BLOCK("Memory_limit:", aSudmissionInformation.mMemoryLimit);
}