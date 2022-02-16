#include "database_query.h"

//UPDATE core_solutions SET result = hh WHERE id = 10
//SELECT * FROM core_solutions WHERE id = 10
//UPDATE core_solutions SET lang = "C:\task1\checker\checker.cpp" WHERE id = 10
//UPDATE core_solutions SET file_name = "2/1.py" WHERE id = 3
//UPDATE core_contests SET time_limit = 1000 WHERE id = 20
//UPDATE core_contests SET memory_limit = 262144 WHERE id = 21

MDatabaseQuery::MDatabaseQuery
(
    std::string aDatabasePath
) :
    mDatabase                   (aDatabasePath),
    mReservedStatementNumber    (0)
{}

MDatabaseQuery::~MDatabaseQuery()
{}

void 
MDatabaseQuery::prepareForTesting
(
    ProblemInformation& aSudmissionInformation
)
{
    getIDInformation(aSudmissionInformation);
    getLimitsInformation(aSudmissionInformation);
    //aSudmissionInformation.id += 200;
    aSudmissionInformation.mTimeLimit /= 1000;
}

void 
MDatabaseQuery::writeResult
(
    int         aID, 
    std::string aResult, 
    int         aTime, 
    int         aMemory
)
{
    WD_LOG("Updating database");

    mDatabase.update("core_solutions", "result = '" + aResult + "'" +
        ", time = " + std::to_string(aTime) +
        ", memory = " + std::to_string(aMemory), "id = " + std::to_string(aID));
    mDatabase.step();
    mDatabase.closeStatment();

    WD_LOG("Database updated?");
    WD_END_LOG;
}

void
MDatabaseQuery::getNextTest(ProblemInformation& aSudmissionInformation, TestLibMessage& aTLM)
{
    WD_LOG("Taking next test");
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
    WD_LOG("Geting test from database");

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
            WD_ERROR(database.0, "Can't open file " + TEST_PATH + std::to_string(cnt));
            continue;//TODO
        }
        if (!ansFile.is_open())
        {
            WD_ERROR(database.0, "Can't open file " + ANSWERS_PATH + std::to_string(cnt));
            continue;
        }

        for (int i = 0; input[i];) taskFile << input[i++];
        for (int i = 0; output[i];) ansFile << output[i++];
    }
    aSudmissionInformation.mTestsCount = cnt;

    mDatabase.closeStatment();

    WD_LOG("Tests taken from pashae");
    WD_END_LOG;
}

void
MDatabaseQuery::prepareTestsStatement
(
    ProblemInformation& aSudmissionInformation
)
{
    aSudmissionInformation.mTestsAreOver = false;
    aSudmissionInformation.mTestsCount = 0;

    WD_LOG("Prepare geting test from database");
    mDatabase.select("core_test", "input, output", "contest_id = " +
        std::to_string(aSudmissionInformation.mContestID), mReservedStatementNumber);
    WD_LOG("I'm ready");
    WD_END_LOG;
}

void 
MDatabaseQuery::getIDInformation
(
    ProblemInformation& aSudmissionInformation
)
{
    WD_LOG("Geting ID and name from database");

    mDatabase.select("core_solutions", "file_name, contest_id", "id = " + std::to_string(aSudmissionInformation.mID));
    mDatabase.step();

    aSudmissionInformation.mSolutionFileName = getString(mDatabase.getTextFromRow(0));
    aSudmissionInformation.mContestID = mDatabase.getInt64FromRow(1);
    mDatabase.closeStatment();

    WD_LOG("File name: " << aSudmissionInformation.mSolutionFileName);
    WD_LOG("Contest ID: " << aSudmissionInformation.mContestID);
    WD_END_LOG;
}

void 
MDatabaseQuery::getLimitsInformation
(
    ProblemInformation& aSudmissionInformation
)
{
    WD_LOG("Geting limits from database");

    mDatabase.select("core_contests", "time_limit, memory_limit, checker", "id = " + std::to_string(aSudmissionInformation.mContestID));
    mDatabase.step();
    aSudmissionInformation.mTimeLimit = mDatabase.getInt64FromRow(0);
    aSudmissionInformation.mMemoryLimit = mDatabase.getInt64FromRow(1);
    aSudmissionInformation.mCheckerFileName = getString(mDatabase.getTextFromRow(2));
    mDatabase.closeStatment();

    WD_LOG("Time limit: " << aSudmissionInformation.mTimeLimit);
    WD_LOG("Memory limit: " << aSudmissionInformation.mMemoryLimit);
    WD_END_LOG;
}